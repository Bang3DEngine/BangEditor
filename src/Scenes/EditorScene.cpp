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

#include "BangEditor/Editor.h"
#include "BangEditor/Console.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/UITabContainer.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/UISceneContainer.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/UIScenePlayContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
}

void EditorScene::Init()
{
    m_editor = new Editor();
    m_editor->Init();

    m_projectManager = new ProjectManager();
    m_scenePlayer = new ScenePlayer();
    m_editorClipboard = new EditorClipboard();

    GameObjectFactory::CreateUIGameObjectInto(this);
    GameObjectFactory::CreateUICanvasInto(this);
    SetName("EditorScene");

    m_mainEditorVL = GameObjectFactory::CreateUIGameObjectNamed("MainEditorVL");
    m_mainEditorVL->AddComponent<UIVerticalLayout>();
    m_mainEditorVL->SetParent(this);

    m_menuBar = GameObject::Create<MenuBar>();
    m_menuBar->GetTransform()->TranslateLocal( Vector3(0, 0, -0.1) );
    m_menuBar->SetParent(m_mainEditorVL);

    m_editSceneGameObjects = new EditSceneGameObjects();

    GameObject *topHLGo = GameObjectFactory::CreateUIGameObject();
    topHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *hlLe = topHLGo->AddComponent<UILayoutElement>();
    hlLe->SetFlexibleSize(Vector2(1));
    topHLGo->SetParent(m_mainEditorVL);

    GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 20)->SetParent(m_mainEditorVL);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObjectNamed("BotHL");
    botHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    botHLLe->SetMinSize( Vector2i(1, 150) );
    botHLLe->SetFlexibleSize( Vector2(1, 0.5f) );
    botHLGo->SetParent(m_mainEditorVL);

    // Inspector, Hierarchy, etc. creation
    p_sceneEditContainer = GameObject::Create<UISceneEditContainer>();
    p_scenePlayContainer = GameObject::Create<UIScenePlayContainer>();
    p_inspector = GameObject::Create<Inspector>();
    p_hierarchy = GameObject::Create<Hierarchy>();
    p_console = GameObject::Create<Console>();
    p_explorer = GameObject::Create<Explorer>();

    // Tab containers creation
    p_leftTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *leftTabContainerLE = p_leftTabContainer->AddComponent<UILayoutElement>();
    leftTabContainerLE->SetFlexibleSize( Vector2(2.0f, 1.0f) );

    p_centerTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *centerTabContainerLE = p_centerTabContainer->AddComponent<UILayoutElement>();
    centerTabContainerLE->SetFlexibleSize( Vector2(6.0f, 1.0f) );

    p_rightTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *rightTabContainerLE = p_rightTabContainer->AddComponent<UILayoutElement>();
    rightTabContainerLE->SetFlexibleSize( Vector2(2.0f, 1.0f) );

    p_botTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *botTabContainerLE = p_botTabContainer->AddComponent<UILayoutElement>();
    botTabContainerLE->SetFlexibleSize( Vector2(1.0f, 1.0f) );

    p_leftTabContainer->SetParent(topHLGo);
    GameObjectFactory::CreateUIVSeparator(LayoutSizeType::Min, 20)->SetParent(topHLGo);
    p_centerTabContainer->SetParent(topHLGo);
    GameObjectFactory::CreateUIVSeparator(LayoutSizeType::Min, 20)->SetParent(topHLGo);
    p_rightTabContainer->SetParent(topHLGo);
    p_botTabContainer->SetParent(botHLGo);
    p_leftTabContainer->AddTab("Hierarchy", p_hierarchy);
    p_centerTabContainer->AddTab("Scene", p_sceneEditContainer);
    p_centerTabContainer->AddTab("Game",  p_scenePlayContainer);
    p_rightTabContainer->AddTab("Inspector", p_inspector);
    p_botTabContainer->AddTab("Explorer", p_explorer);
    p_botTabContainer->AddTab("Console", p_console);
    p_botTabContainer->SetCurrentTabChild(p_explorer);

    // Editor cam creation
    Camera *cam = AddComponent<Camera>();
    cam->AddRenderPass(RenderPass::Overlay);
    SetCamera(cam);
    GetCamera()->SetClearColor(Color::LightGray);
    GetCamera()->SetRenderSelectionBuffer(false);

    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);

    ScenePlayer::StopScene();
}

EditorScene::~EditorScene()
{
    delete m_editor;
    delete m_scenePlayer;
    delete m_projectManager;
    delete m_editSceneGameObjects;
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

        bool updateOpenScene =
                (Editor::GetEditorPlayState() == EditorPlayState::Playing);
        SceneManager::OnNewFrame(openScene, updateOpenScene);

        GetEditSceneGameObjects()->Update();

        EditorSceneManager::SetActiveScene(this);
        GetSceneEditContainer()->HandleSelection();
        EditorSceneManager::SetActiveScene(openScene);

        UnBindOpenScene();
    }

    // Set scene tab name
    String sceneTabName = "Scene";
    if (openScene)
    {
        Path openScenePath = EditorSceneManager::GetOpenScenePath();
        sceneTabName += " - " + openScenePath.GetName();
        if (Editor::IsEditingScene() &&
            !SceneOpenerSaver::GetInstance()->IsCurrentSceneSaved())
        {
            sceneTabName += " (*)";
        }
    }
    p_centerTabContainer->SetTabTitle(p_sceneEditContainer, sceneTabName);

}

void EditorScene::OnResize(int newWidth, int newHeight)
{
    EditorSceneManager::SetActiveScene(this);
    Scene::OnResize(newWidth, newHeight);

    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        BindOpenScene();
        openScene->OnResize(newWidth, newHeight);
        UnBindOpenScene();
    }
}

void EditorScene::RenderOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        BindOpenScene();

        GetEditSceneGameObjects()->OnBeginRender(openScene);
        GetSceneEditContainer()->RenderIfNeeded();
        GetEditSceneGameObjects()->OnEndRender(openScene);

        GetScenePlayContainer()->RenderIfNeeded();

        UnBindOpenScene();
    }
}

void EditorScene::SetViewportForOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        Camera *openSceneCam = openScene->GetCamera();
        if (openSceneCam)
        {
            Rect ndcRect = GetOpenSceneWindowRectNDC();
            Rect ndcRectNorm = ndcRect * 0.5f + 0.5f;
            Recti vpRectPx(
               Vector2i(ndcRectNorm.GetMin() * Vector2(GL::GetViewportSize())),
               Vector2i(ndcRectNorm.GetMax() * Vector2(GL::GetViewportSize())));

            GL::SetViewport(vpRectPx);
        }
    }
}

void EditorScene::SetOpenScene(Scene *openScene, bool destroyPreviousScene)
{
    if (openScene != GetOpenScene())
    {
        if (GetOpenScene())
        {
            if (destroyPreviousScene) { GameObject::Destroy(GetOpenScene()); }
            GetOpenScene()->DestroyPending();
        }

        p_openScene = openScene;
        if (GetOpenScene())
        {
            EventEmitter<IEditorOpenSceneListener>::PropagateToListeners(
                        &IEditorOpenSceneListener::OnOpenScene, GetOpenScene());

            BindOpenScene();
            GetOpenScene()->SetFirstFoundCamera();
            GetOpenScene()->InvalidateCanvas();
            UnBindOpenScene();
        }
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

Rect EditorScene::GetOpenSceneWindowRectNDC() const
{
    return p_sceneEditContainer->GetSceneImageRectNDC();
}

void EditorScene::RenderAndBlitToWindow()
{
    Window *window = Window::GetActive();
    window->Clear();

    GEngine *gEngine = GEngine::GetActive();
    RenderOpenScene();
    gEngine->Render(this, GetCamera());
    window->BlitToWindow(GetCamera());
}

void EditorScene::BindOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        EditorSceneManager::SetActiveScene(openScene);
        PushGLViewport();
        SetViewportForOpenScene();
    }
}

void EditorScene::UnBindOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        PopGLViewport();
    }
    EditorSceneManager::SetActiveScene(this);
}

MenuBar *EditorScene::GetMenuBar() const { return m_menuBar; }
Console *EditorScene::GetConsole() const { return p_console; }
Explorer *EditorScene::GetExplorer() const { return p_explorer; }
Inspector *EditorScene::GetInspector() const { return p_inspector; }
Hierarchy *EditorScene::GetHierarchy() const { return p_hierarchy; }
ScenePlayer *EditorScene::GetScenePlayer() const { return m_scenePlayer; }
ProjectManager *EditorScene::GetProjectManager() const
{
    return m_projectManager;
}

EditorClipboard *EditorScene::GetEditorClipboard() const
{
    return m_editorClipboard;
}

UITabContainer *EditorScene::GetSceneTabContainer() const
{
    return p_centerTabContainer;
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

Editor *EditorScene::GetEditor() const { return m_editor; }

void EditorScene::PushGLViewport()
{
    m_prevGLViewport = GL::GetViewportRect();
}

void EditorScene::PopGLViewport()
{
    GL::SetViewport(m_prevGLViewport);
}

void EditorScene::OnPlayStateChanged(EditorPlayState,
                                     EditorPlayState newPlayState)
{
    // Change tab when play/stop
    switch (newPlayState)
    {
        case EditorPlayState::Editing:
            p_centerTabContainer->SetCurrentTabChild( p_sceneEditContainer );
        break;

        case EditorPlayState::Playing:
            p_centerTabContainer->SetCurrentTabChild( p_scenePlayContainer );
        break;

        default: break;
    }
}
