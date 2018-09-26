#include "BangEditor/EditorScene.h"

#include <array>

#include "Bang/GL.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/UIMask.h"
#include "Bang/Window.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UICanvas.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/AudioManager.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIDirLayoutMovableSeparator.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Console.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/UITabStation.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/UITabContainer.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/UISceneContainer.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/UIScenePlayContainer.h"
#include "BangEditor/EditorBehaviourManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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
    this->RemoveComponent( GetComponent<Camera>() );
    SetName("EditorScene");

    m_mainEditorVLGo = GameObjectFactory::CreateUIGameObjectNamed("MainEditorVL");
    UIVerticalLayout *mainEdVL = m_mainEditorVLGo->AddComponent<UIVerticalLayout>();
    mainEdVL->SetPaddings(5, 30, 5, 0);
    m_mainEditorVLGo->SetParent(this);

    m_menuBar = GameObject::Create<MenuBar>();
    m_menuBar->GetTransform()->TranslateLocal( Vector3(0, 0, -0.1) );
    m_menuBar->SetParent(m_mainEditorVLGo);

    // GameObjectFactory::CreateUIVSpacer(LayoutSizeType::MIN, 5)->
    //                    SetParent(m_mainEditorVLGo);

    m_editSceneGameObjects = new EditSceneGameObjects();

    // GameObject *topHLGo = GameObjectFactory::CreateUIGameObject();
    // topHLGo->AddComponent<UIHorizontalLayout>();
    // UILayoutElement *topHLLe = topHLGo->AddComponent<UILayoutElement>();
    // topHLLe->SetLayoutPriority(1);
    // topHLLe->SetFlexibleSize(Vector2(1, 1.5f));
    // topHLGo->SetParent(m_mainEditorVLGo);

    // GameObjectFactory::CreateUIDirLayoutMovableVSeparator()->GetGameObject()->
    //                    SetParent(m_mainEditorVLGo);

    // GameObject *botHLGo = GameObjectFactory::CreateUIGameObjectNamed("BotHL");
    // botHLGo->AddComponent<UIHorizontalLayout>();
    // UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    // botHLLe->SetLayoutPriority(1);
    // botHLLe->SetMinSize( Vector2i(1, 300) );
    // botHLLe->SetFlexibleSize( Vector2(1, 1) );
    // botHLGo->SetParent(m_mainEditorVLGo);

    // Inspector, Hierarchy, etc. creation
    p_sceneEditContainer = GameObject::Create<UISceneEditContainer>();
    p_scenePlayContainer = GameObject::Create<UIScenePlayContainer>();
    p_inspector = GameObject::Create<Inspector>();
    p_hierarchy = GameObject::Create<Hierarchy>();
    p_console = GameObject::Create<Console>();
    p_explorer = GameObject::Create<Explorer>();

    p_tabStation = GameObject::Create<UITabStation>();
    p_tabStation->SetParent(m_mainEditorVLGo);

    p_tabStation->GetTabContainer()->AddTab("Scene", p_sceneEditContainer);
    p_tabStation->GetTabContainer()->AddTab("Game",  p_scenePlayContainer);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::LEFT)->
                  GetTabContainer()->AddTab("Hierarchy", p_hierarchy);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::RIGHT)->
                  GetTabContainer()->AddTab("Inspector", p_inspector);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)->
                  GetTabContainer()->AddTab("Explorer", p_explorer);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)->
                  GetTabContainer()->AddTab("Console", p_console);

    p_tabStation->GetChildStationAndCreateIfNeeded(Side::LEFT)->GetParent()->
                  GetComponent<UILayoutElement>()->SetFlexibleWidth(0.1f);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::RIGHT)->GetParent()->
                  GetComponent<UILayoutElement>()->SetFlexibleWidth(0.2f);
    p_tabStation->GetChildStationAndCreateIfNeeded(Side::BOT)->GetParent()->
                  GetComponent<UILayoutElement>()->SetFlexibleHeight(0.3f);

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

    cam->SetClearColor( Color::White.WithValue(0.8f) );
    cam->SetClearMode(CameraClearMode::COLOR);
    cam->SetRenderFlags( RenderFlags(RenderFlag::NONE).
                         SetOn(RenderFlag::CLEAR_COLOR).
                         SetOn(RenderFlag::CLEAR_DEPTH_STENCIL) );
    SetCamera(cam);


    EditorWindow *editorWindow = SCAST<EditorWindow*>(Window::GetActive());
    editorWindow->EventEmitter<IEventsWindow>::RegisterListener(this);

    ScenePlayer::GetInstance()->EventEmitter<IEventsScenePlayer>::RegisterListener(this);
    SceneManager::GetActive()->EventEmitter<IEventsSceneManager>::RegisterListener(this);
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

void EditorScene::BeforeRender()
{
    Scene::BeforeRender();
    RenderOpenSceneIfNeeded();
}

void EditorScene::Update()
{
    GetScenePlayer()->Update();

    EditorSceneManager::SetActiveScene(this);
    Scene::Update();

    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        BindOpenScene();

        Input::Context openSceneInputContext;
        openSceneInputContext.focus = GetScenePlayContainer()->GetFocusable();
        openSceneInputContext.rect  = AARecti(GetScenePlayContainer()->
                                              GetSceneImage()->
                                              GetRectTransform()->
                                              GetViewportAARect());
        Input::SetContext(openSceneInputContext);

        bool updateOpenScene = (ScenePlayer::GetPlayState() == PlayState::PLAYING);
        SceneManager::OnNewFrame(openScene, updateOpenScene);

        openSceneInputContext.focus = GetSceneEditContainer()->GetFocusable();
        openSceneInputContext.rect  = AARecti(GetSceneEditContainer()->
                                              GetSceneImage()->
                                              GetRectTransform()->
                                              GetViewportAARect());
        Input::SetContext(openSceneInputContext);
        GetEditSceneGameObjects()->Update();

        Input::ClearContext();
        UnBindOpenScene();
    }

    // Set scene tab name
    String sceneTabName = "Scene - ";
    if (openScene)
    {
        Path loadedScenePath = SceneOpenerSaver::GetInstance()->GetLoadedScenePath();
        sceneTabName += loadedScenePath.GetName();
        if ( Editor::IsEditingScene() &&
            !SceneOpenerSaver::GetInstance()->IsCurrentSceneSaved())
        {
            sceneTabName += " (*)";
        }
    }
    p_tabStation->FindTabStationOf(p_sceneEditContainer)->
                  GetTabContainer()->SetTabTitle(p_sceneEditContainer, sceneTabName);
}

void EditorScene::OnResize(int newWidth, int newHeight)
{
    Scene::OnResize(newWidth, newHeight);
    if ( GetOpenScene() )
    {
        BindOpenScene();
        GetOpenScene()->OnResize(newWidth, newHeight);
        UnBindOpenScene();
    }
}

void EditorScene::RenderOpenSceneIfNeeded()
{
    if ( GetOpenScene() )
    {
        BindOpenScene();

        GetEditSceneGameObjects()->OnBeginRender( GetOpenScene() );
        GetSceneEditContainer()->RenderIfNeeded();
        GetEditSceneGameObjects()->OnEndRender( GetOpenScene() );

        GetScenePlayContainer()->RenderIfNeeded();

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
            GetOpenScene()->EventEmitter<IEventsDestroy>::RegisterListener(this);

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

AARect EditorScene::GetOpenSceneWindowRectNDC() const
{
    return p_sceneEditContainer->GetSceneImageAARectNDC();
}

void EditorScene::BindOpenScene()
{
    if ( GetOpenScene() )
    {
        EditorSceneManager::SetActiveScene( GetOpenScene() );
    }
}

void EditorScene::UnBindOpenScene()
{
    if ( GetOpenScene() )
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
            p_tabStation->FindTabStationOf( GetSceneEditContainer() )->
                          GetTabContainer()->
                          SetCurrentTabChild( GetSceneEditContainer() );
            UICanvas::GetActive(this)->SetFocus(
                        GetSceneEditContainer()->GetFocusable());
        break;

        case PlayState::PLAYING:
            p_tabStation->FindTabStationOf( GetScenePlayContainer() )->
                          GetTabContainer()->
                          SetCurrentTabChild( GetScenePlayContainer() );
            UICanvas::GetActive(this)->SetFocus(
                        GetScenePlayContainer()->GetFocusable());
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
