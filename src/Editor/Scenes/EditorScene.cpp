#include "BangEditor/EditorScene.h"

#include "Bang/Alignment.h"
#include "Bang/AudioManager.h"
#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Flags.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsFileTracker.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Input.h"
#include "Bang/Path.h"
#include "Bang/RectTransform.h"
#include "Bang/RenderFlags.h"
#include "Bang/RenderPass.h"
#include "Bang/SceneManager.h"
#include "Bang/Transform.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/Window.h"
#include "BangEditor/AnimatorSMEditor.h"
#include "BangEditor/Console.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorBehaviourManager.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/IEventsTabHeader.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/UIScenePlayContainer.h"
#include "BangEditor/UITabContainer.h"
#include "BangEditor/UITabHeader.h"
#include "BangEditor/UITabStation.h"
#include "BangEditor/UndoRedoManager.h"

using namespace Bang;
using namespace BangEditor;

EditorScene::EditorScene()
{
}

void EditorScene::Init()
{
    m_projectManager = new ProjectManager();
    m_scenePlayer = new ScenePlayer();
    m_editorClipboard = new EditorClipboard();
    m_undoRedoManager = new UndoRedoManager();
    m_sceneOpenerSaver = new SceneOpenerSaver();
    m_editorFileTracker = new EditorFileTracker();

    GameObjectFactory::CreateUISceneInto(this);
    this->RemoveComponent(GetComponent<Camera>());
    SetName("EditorScene");

    m_mainEditorVLGo =
        GameObjectFactory::CreateUIGameObjectNamed("MainEditorVL");
    UIVerticalLayout *mainEdVL =
        m_mainEditorVLGo->AddComponent<UIVerticalLayout>();
    mainEdVL->SetPaddings(5, 30, 5, 0);
    m_mainEditorVLGo->SetParent(this);

    m_menuBar = new MenuBar();
    m_menuBar->GetTransform()->TranslateLocal(Vector3(0, 0, -0.1));
    m_menuBar->SetParent(m_mainEditorVLGo);

    // GameObjectFactory::CreateUIVSpacer(LayoutSizeType::MIN, 5)->
    //                    SetParent(m_mainEditorVLGo);

    m_editSceneGameObjects = new EditSceneGameObjects();

    // Inspector, Hierarchy, etc. creation
    p_sceneEditContainer = new UISceneEditContainer();
    p_scenePlayContainer = new UIScenePlayContainer();
    p_animatorSMEditor = new AnimatorSMEditor();
    p_inspector = new Inspector();
    p_hierarchy = new Hierarchy();
    p_console = new Console();
    p_explorer = new Explorer();

    p_tabStation = new UITabStation();
    p_tabStation->SetParent(m_mainEditorVLGo);

    p_tabStation->GetTabContainer()->AddTab("Scene", p_sceneEditContainer);
    p_tabStation->GetTabContainer()->AddTab("Game", p_scenePlayContainer);
    p_tabStation->GetTabContainer()->AddTab("AnimatorSM Editor",
                                            p_animatorSMEditor);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::LEFT)
        ->GetTabContainer()
        ->AddTab("Hierarchy", p_hierarchy);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::RIGHT)
        ->GetTabContainer()
        ->AddTab("Inspector", p_inspector);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)
        ->GetTabContainer()
        ->AddTab("Explorer", p_explorer);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)
        ->GetTabContainer()
        ->AddTab("Console", p_console);

    p_tabStation->GetChildStationAndCreateIfNeeded(Side::LEFT)
        ->GetParent()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleWidth(0.1f);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::RIGHT)
        ->GetParent()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleWidth(0.2f);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)
        ->GetParent()
        ->GetComponent<UILayoutElement>()
        ->SetFlexibleHeight(0.3f);

    // Editor cam creation
    Camera *cam = GameObjectFactory::CreateUICameraInto(this);
    cam->RemoveRenderPass(RenderPass::SCENE);
    cam->RemoveRenderPass(RenderPass::SCENE_POSTPROCESS);
    cam->RemoveRenderPass(RenderPass::SCENE_TRANSPARENT);
    cam->RemoveRenderPass(RenderPass::CANVAS_POSTPROCESS);
    cam->RemoveRenderPass(RenderPass::OVERLAY);
    cam->RemoveRenderPass(RenderPass::OVERLAY_POSTPROCESS);

#ifdef DEBUG
    cam->AddRenderPass(RenderPass::OVERLAY);
#endif

    cam->SetClearColor(Color::White().WithValue(0.8f));
    cam->SetClearMode(CameraClearMode::COLOR);
    cam->SetRenderFlags(RenderFlags(RenderFlag::NONE)
                            .SetOn(RenderFlag::CLEAR_COLOR)
                            .SetOn(RenderFlag::CLEAR_DEPTH_STENCIL));
    SetCamera(cam);

    EditorWindow *editorWindow = SCAST<EditorWindow *>(Window::GetActive());
    editorWindow->EventEmitter<IEventsWindow>::RegisterListener(this);

    ScenePlayer::GetInstance()
        ->EventEmitter<IEventsScenePlayer>::RegisterListener(this);
    SceneManager::GetActive()
        ->EventEmitter<IEventsSceneManager>::RegisterListener(this);
    GetEditorFileTracker()->EventEmitter<IEventsFileTracker>::RegisterListener(
        EditorBehaviourManager::GetActive());

    ScenePlayer::StopScene();

    AudioManager::SetPlayOnStartBlocked(true);
}

EditorScene::~EditorScene()
{
    delete m_scenePlayer;
    delete m_projectManager;
    delete m_undoRedoManager;
    delete m_sceneOpenerSaver;
    delete m_editorFileTracker;
    delete m_editSceneGameObjects;
}

void EditorScene::Update()
{
    Scene::Update();
    EditorSceneManager::SetActiveScene(this);
    GetScenePlayer()->Update();

    String sceneTabName = "Scene - ";
    if (Scene *openScene = GetOpenScene())
    {
        if (ScenePlayer::GetPlayState() == PlayState::PLAYING)
        {
            // Update open scene if playing
            BindOpenScene();

            Input::Context openSceneInputContext;
            openSceneInputContext.focus =
                GetScenePlayContainer()->GetFocusable();
            openSceneInputContext.rect = AARecti(GetScenePlayContainer()
                                                     ->GetSceneImage()
                                                     ->GetRectTransform()
                                                     ->GetViewportAARect());
            Input::SetContext(openSceneInputContext);

            SceneManager::OnNewFrame(openScene);

            UnBindOpenScene();
        }

        // Set scene tab name
        Path loadedScenePath =
            SceneOpenerSaver::GetInstance()->GetLoadedScenePath();
        sceneTabName += loadedScenePath.GetName();
        if (Editor::IsEditingScene() &&
            !SceneOpenerSaver::GetInstance()->IsCurrentSceneSaved())
        {
            sceneTabName += " (*)";
        }
    }

    // Update editor scene stuff
    Input::Context editSceneInputContext;
    editSceneInputContext.focus = GetSceneEditContainer()->GetFocusable();
    editSceneInputContext.rect = AARecti(GetSceneEditContainer()
                                             ->GetSceneImage()
                                             ->GetRectTransform()
                                             ->GetViewportAARect());
    Input::SetContext(editSceneInputContext);
    GetEditSceneGameObjects()->Update();
    Input::ClearContext();

    if (UITabStation *sceneEditTabStation =
            p_tabStation->FindTabStationOf(p_sceneEditContainer))
    {
        sceneEditTabStation->GetTabContainer()->SetTabTitle(
            p_sceneEditContainer, sceneTabName);
    }
}

void EditorScene::OnResize(int newWidth, int newHeight)
{
    Scene::OnResize(newWidth, newHeight);
    if (GetOpenScene())
    {
        BindOpenScene();
        GetOpenScene()->OnResize(newWidth, newHeight);
        UnBindOpenScene();
    }
}

void EditorScene::SetOpenScene(Scene *openScene)
{
    if (openScene != GetOpenScene())
    {
        p_openScene = openScene;
        if (GetOpenScene())
        {
            GetOpenScene()->EventEmitter<IEventsDestroy>::RegisterListener(
                this);

            BindOpenScene();
            GetOpenScene()->InvalidateCanvas();
            UnBindOpenScene();
        }
    }
}

void EditorScene::OnTabHeaderClicked(UITabHeader *header)
{
    if (header->GetTitle().BeginsWith("Scene"))
    {
        UICanvas::GetActive(this)->SetFocus(
            GetSceneEditContainer()->GetFocusable());
    }
    else if (header->GetTitle() == "Game")
    {
        UICanvas::GetActive(this)->SetFocus(
            GetScenePlayContainer()->GetFocusable());
    }
}

void EditorScene::OnFocusGained(Window *w)
{
    BANG_UNUSED(w);
    GetEditorFileTracker()->CheckFiles();
}

void EditorScene::OnFocusLost(Window *w)
{
    BANG_UNUSED(w);
}

void EditorScene::OnSceneLoaded(Scene *scene, const Path &)
{
    if (scene != this)
    {
        SetOpenScene(scene);
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

void EditorScene::OpenTab(GameObject *tabbedChild)
{
    if (auto playTabStation = p_tabStation->FindTabStationOf(tabbedChild))
    {
        playTabStation->GetTabContainer()->SetCurrentTabChild(tabbedChild);
    }
    UICanvas::GetActive(this)->SetFocus(
        tabbedChild->GetComponent<UIFocusable>());
}

AARect EditorScene::GetOpenSceneWindowRectNDC() const
{
    return p_sceneEditContainer->GetSceneImageAARectNDC();
}

void EditorScene::BindOpenScene()
{
    if (GetOpenScene())
    {
        EditorSceneManager::SetActiveScene(GetOpenScene());
    }
}

void EditorScene::UnBindOpenScene()
{
    if (GetOpenScene())
    {
        EditorSceneManager::SetActiveScene(this);
    }
}

MenuBar *EditorScene::GetMenuBar() const
{
    return m_menuBar;
}
Console *EditorScene::GetConsole() const
{
    return p_console;
}
Explorer *EditorScene::GetExplorer() const
{
    return p_explorer;
}
Inspector *EditorScene::GetInspector() const
{
    return p_inspector;
}
Hierarchy *EditorScene::GetHierarchy() const
{
    return p_hierarchy;
}
ScenePlayer *EditorScene::GetScenePlayer() const
{
    return m_scenePlayer;
}
ProjectManager *EditorScene::GetProjectManager() const
{
    return m_projectManager;
}

AnimatorSMEditor *EditorScene::GetAnimatorSMEditor() const
{
    return p_animatorSMEditor;
}

EditorClipboard *EditorScene::GetEditorClipboard() const
{
    return m_editorClipboard;
}

UndoRedoManager *EditorScene::GetUndoRedoManager() const
{
    return m_undoRedoManager;
}

SceneOpenerSaver *EditorScene::GetSceneOpenerSaver() const
{
    return m_sceneOpenerSaver;
}

EditorFileTracker *EditorScene::GetEditorFileTracker() const
{
    return m_editorFileTracker;
}

UISceneEditContainer *EditorScene::GetSceneEditContainer() const
{
    return p_sceneEditContainer;
}

UIScenePlayContainer *EditorScene::GetScenePlayContainer() const
{
    return p_scenePlayContainer;
}

EditSceneGameObjects *EditorScene::GetEditSceneGameObjects() const
{
    return m_editSceneGameObjects;
}

void EditorScene::OnPlayStateChanged(PlayState previousPlayState,
                                     PlayState newPlayState)
{
    BANG_UNUSED(previousPlayState);

    // Change tab when play/stop
    switch (newPlayState)
    {
        case PlayState::EDITING:
        {
            OpenTab(GetSceneEditContainer());
        }
        break;

        case PlayState::PLAYING:
            if (previousPlayState == PlayState::JUST_BEFORE_PLAYING)
            {
                OpenTab(GetScenePlayContainer());
            }
            break;

        default: break;
    }
}

void EditorScene::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    Scene::OnDestroyed(object);

    if (object == GetOpenScene())
    {
        p_openScene = nullptr;
    }
}
