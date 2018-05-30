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
#include "BangEditor/ProjectManager.h"
#include "BangEditor/UITabContainer.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/UISceneContainer.h"
#include "BangEditor/BehaviourTracker.h"
#include "BangEditor/EditorFileTracker.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/UIScenePlayContainer.h"
#include "BangEditor/EditorDragDropManager.h"

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
    m_sceneOpenerSaver = new SceneOpenerSaver();
    m_editorFileTracker = new EditorFileTracker();

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
    m_editorDragDropManager = new EditorDragDropManager();

    GameObject *topHLGo = GameObjectFactory::CreateUIGameObject();
    topHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *topHLLe = topHLGo->AddComponent<UILayoutElement>();
    topHLLe->SetLayoutPriority(1);
    topHLLe->SetFlexibleSize(Vector2(1, 1.5f));
    topHLGo->SetParent(m_mainEditorVL);

    GameObjectFactory::CreateUIDirLayoutMovableVSeparator()->GetGameObject()->
                       SetParent(m_mainEditorVL);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObjectNamed("BotHL");
    botHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    botHLLe->SetLayoutPriority(1);
    botHLLe->SetMinSize( Vector2i(1, 300) );
    botHLLe->SetFlexibleSize( Vector2(1, 1) );
    botHLGo->SetParent(m_mainEditorVL);

    // Inspector, Hierarchy, etc. creation
    p_sceneEditContainer = GameObject::Create<UISceneEditContainer>();
    p_scenePlayContainer = GameObject::Create<UIScenePlayContainer>();
    p_inspector = GameObject::Create<Inspector>();
    p_hierarchy = GameObject::Create<Hierarchy>();
    p_console = GameObject::Create<Console>();
    p_explorer = GameObject::Create<Explorer>();

    // Tab containers creation
    p_topLeftTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *topLeftTabContainerLE = p_topLeftTabContainer->AddComponent<UILayoutElement>();
    topLeftTabContainerLE->SetMinSize( Vector2i(300) );
    topLeftTabContainerLE->SetFlexibleSize( Vector2(0.0f, 1.0f) );

    p_topCenterTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *topCenterTabContainerLE = p_topCenterTabContainer->AddComponent<UILayoutElement>();
    topCenterTabContainerLE->SetMinSize( Vector2i(500, 250) );
    topCenterTabContainerLE->SetFlexibleSize( Vector2(3.0f, 1.0f) );

    p_topRightTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *topRightTabContainerLE = p_topRightTabContainer->AddComponent<UILayoutElement>();
    topRightTabContainerLE->SetMinSize( Vector2i(400) );
    topRightTabContainerLE->SetFlexibleSize( Vector2(0.0f, 1.0f) );

    p_botLeftTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *botLeftTabContainerLE = p_botLeftTabContainer->AddComponent<UILayoutElement>();
    botLeftTabContainerLE->SetMinSize( Vector2i(500, 250) );
    botLeftTabContainerLE->SetFlexibleSize( Vector2(1.0f, 1.0f) );

    p_botRightTabContainer = GameObject::Create<UITabContainer>();
    UILayoutElement *botRightTabContainerLE = p_botRightTabContainer->AddComponent<UILayoutElement>();
    botRightTabContainerLE->SetMinSize( Vector2i(250) );
    botRightTabContainerLE->SetFlexibleSize( Vector2(0.35f, 1.0f) );

    p_topLeftTabContainer->SetParent(topHLGo);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->GetGameObject()->
                       SetParent(topHLGo);
    p_topCenterTabContainer->SetParent(topHLGo);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->GetGameObject()->
                       SetParent(topHLGo);
    p_topRightTabContainer->SetParent(topHLGo);

    p_botLeftTabContainer->SetParent(botHLGo);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->GetGameObject()->
                       SetParent(botHLGo);
    p_botRightTabContainer->SetParent(botHLGo);

    p_topLeftTabContainer->AddTab("Hierarchy", p_hierarchy);
    p_topCenterTabContainer->AddTab("Scene", p_sceneEditContainer);
    p_topCenterTabContainer->AddTab("Game",  p_scenePlayContainer);
    p_topRightTabContainer->AddTab("Inspector", p_inspector);
    p_botLeftTabContainer->AddTab("Explorer", p_explorer);
    p_botRightTabContainer->AddTab("Console", p_console);

    // Editor cam creation
    Camera *cam = GameObjectFactory::CreateUICameraInto(this);
    cam->RemoveRenderPass(RenderPass::SCENE);
    cam->RemoveRenderPass(RenderPass::SCENE_POSTPROCESS);
    cam->RemoveRenderPass(RenderPass::CANVAS_POSTPROCESS);
    cam->RemoveRenderPass(RenderPass::OVERLAY);
    cam->RemoveRenderPass(RenderPass::OVERLAY_POSTPROCESS);
    SetCamera(cam);

    ScenePlayer::GetInstance()->EventEmitter<IScenePlayerListener>::RegisterListener(this);
    SceneManager::GetActive()->EventEmitter<ISceneManagerListener>::RegisterListener(this);

    ScenePlayer::StopScene();
}

EditorScene::~EditorScene()
{
    delete m_scenePlayer;
    delete m_projectManager;
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
    GetEditorFileTracker()->GetFileTracker()->Update(false);
    GetEditorFileTracker()->GetBehaviourTracker()->Update(false);

    EditorSceneManager::SetActiveScene(this);
    Scene::Update();

    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        BindOpenScene();

        bool updateOpenScene = (ScenePlayer::GetPlayState() == PlayState::PLAYING);
        SceneManager::OnNewFrame(openScene, updateOpenScene);

        GetEditSceneGameObjects()->Update();

        GetSceneEditContainer()->HandleSelection();
        EditorSceneManager::SetActiveScene(this);

        UnBindOpenScene();
    }

    // Set scene tab name
    String sceneTabName = "Scene";
    if (openScene)
    {
        Path loadedScenePath = SceneOpenerSaver::GetInstance()->GetLoadedScenePath();
        sceneTabName += " - " + loadedScenePath.GetName();
        if ( Editor::IsEditingScene() &&
            !SceneOpenerSaver::GetInstance()->IsCurrentSceneSaved())
        {
            sceneTabName += " (*)";
        }
    }
    p_topCenterTabContainer->SetTabTitle(p_sceneEditContainer, sceneTabName);
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

void EditorScene::SetViewportForOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        Camera *openSceneCam = openScene->GetCamera();
        if (openSceneCam)
        {
            AARect ndcRect = GetOpenSceneWindowRectNDC();
            AARect ndcRectNorm = ndcRect * 0.5f + 0.5f;
            AARecti vpRectPx(
               Vector2i(ndcRectNorm.GetMin() * Vector2(GL::GetViewportSize())),
               Vector2i(ndcRectNorm.GetMax() * Vector2(GL::GetViewportSize())));

            GL::SetViewport(vpRectPx);
        }
    }
}

void EditorScene::SetOpenScene(Scene *openScene)
{
    if (openScene != GetOpenScene())
    {
        p_openScene = openScene;
        if (GetOpenScene())
        {
            GetOpenScene()->EventEmitter<IDestroyListener>::RegisterListener(this);

            BindOpenScene();
            GetOpenScene()->SetFirstFoundCamera();
            GetOpenScene()->InvalidateCanvas();
            UnBindOpenScene();
        }
    }
}

void EditorScene::OnSceneLoaded(Scene *scene, const Path &)
{
    if (scene != this) { SetOpenScene(scene); }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

AARect EditorScene::GetOpenSceneWindowRectNDC() const
{
    return p_sceneEditContainer->GetSceneImageRectNDC();
}

void EditorScene::BindOpenScene()
{
    if ( GetOpenScene() )
    {
        PushGLViewport();
        SetViewportForOpenScene();
        EditorSceneManager::SetActiveScene( GetOpenScene() );
    }
}

void EditorScene::UnBindOpenScene()
{
    if ( GetOpenScene() )
    {
        EditorSceneManager::SetActiveScene(this);
        PopGLViewport();
    }
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
    return p_topCenterTabContainer;
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

EditorDragDropManager *EditorScene::GetEditorDragDropManager() const
{
    return m_editorDragDropManager;
}

void EditorScene::PushGLViewport()
{
    m_viewportsStack.push(GL::GetViewportRect());
}

void EditorScene::PopGLViewport()
{
    if (!m_viewportsStack.empty())
    {
        GL::SetViewport( m_viewportsStack.top() );
        m_viewportsStack.pop();
    }
}

void EditorScene::OnPlayStateChanged(PlayState previousPlayState,
                                     PlayState newPlayState)
{
    (void)(previousPlayState);

    // Change tab when play/stop
    switch (newPlayState)
    {
        case PlayState::EDITING:
            p_topCenterTabContainer->SetCurrentTabChild( p_sceneEditContainer );
        break;

        case PlayState::PLAYING:
            p_topCenterTabContainer->SetCurrentTabChild( p_scenePlayContainer );
        break;

        default: break;
    }
}

void EditorScene::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    Scene::OnDestroyed(object);

    if (object == GetOpenScene())
    {
        p_openScene = nullptr;
    }
}
