#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/Window.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UICanvas.h"
#include "Bang/Texture2D.h"
#include "Bang/GameObject.h"
#include "Bang/UIBorderRect.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutManager.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Console.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/UISceneContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
    SetName("EditorScene");
    GameObjectFactory::CreateUIGameObjectInto(this);
    GameObjectFactory::CreateUICanvasInto(this);

    m_mainEditorVL = GameObjectFactory::CreateUIGameObject("MainEditorVL");
    UIVerticalLayout *vl = m_mainEditorVL->AddComponent<UIVerticalLayout>();
    m_mainEditorVL->SetParent(this);

    m_menuBar = ObjectManager::Create<MenuBar>();
    m_menuBar->GetTransform()->TranslateLocal( Vector3(0, 0, -0.1) );
    m_menuBar->SetParent(m_mainEditorVL);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *hlLe = hlGo->AddComponent<UILayoutElement>();
    hlLe->SetFlexibleSize(Vector2(1));
    hlGo->SetParent(m_mainEditorVL);

    m_mainEditorVL->SetAsChild(
              GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Min, 10));

    m_sceneContainer = ObjectManager::Create<UISceneContainer>();
    m_sceneContainer->SetParent(hlGo);

    m_inspector = ObjectManager::Create<Inspector>();
    m_inspector->SetParent(hlGo);

    m_hierarchy = ObjectManager::Create<Hierarchy>();
    m_hierarchy->SetParent(hlGo, 0);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject("BotHL");
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    botHLLe->SetMinSize( Vector2i(1, 150) );
    botHLLe->SetFlexibleSize( Vector2(1) );
    botHLGo->SetParent(m_mainEditorVL);

    m_console = ObjectManager::Create<Console>();
    m_console->SetParent(botHLGo);

    m_explorer = ObjectManager::Create<Explorer>();
    m_explorer->SetParent(botHLGo);

    Camera *cam = AddComponent<Camera>();
    SetCamera(cam);
    GetCamera()->SetClearColor(Color::LightGray);
}

EditorScene::~EditorScene()
{
}

void EditorScene::PreUpdate()
{
    Scene::PreUpdate();
    if (GetOpenScene()) { GetOpenScene()->PreUpdate(); }
}

void EditorScene::Update()
{
    Scene::Update();
    if (GetOpenScene()) { GetOpenScene()->Update(); }
}

void EditorScene::PostUpdate()
{
    Scene::PostUpdate();
    if (GetOpenScene()) { GetOpenScene()->PostUpdate(); }
}

void EditorScene::OnResize(int newWidth, int newHeight)
{
    Scene::OnResize(newWidth, newHeight);
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        SaveGLViewport();
        SetViewportForOpenScene();
        openScene->InvalidateCanvas();
        LoadGLViewport();
    }
}

void EditorScene::RenderOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        SaveGLViewport();
        SetViewportForOpenScene();
        GEngine::GetInstance()->Render(openScene);
        LoadGLViewport();
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
            Rect ndcRect = GetOpenSceneRectNDC();
            Rect ndcRectNorm = ndcRect * 0.5f + 0.5f;
            Recti vpRectPx(
               Vector2i(ndcRectNorm.GetMin() * Vector2(GL::GetViewportSize())),
               Vector2i(ndcRectNorm.GetMax() * Vector2(GL::GetViewportSize())));

            GL::SetViewport(vpRectPx);
        }
    }
}

void EditorScene::SetOpenScene(Scene *openScene)
{
    p_openScene = openScene;
    if (p_openScene)
    {
        p_openScene->SetFirstFoundCameraOrDefaultOne();
        p_openScene->InvalidateCanvas();
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

Rect EditorScene::GetOpenSceneRectNDC() const
{
    return m_sceneContainer->GetImageRect();
}

void EditorScene::RenderAndBlitToScreen()
{
    Window *window = Window::GetCurrent();
    window->Clear();

    UILayoutManager::RebuildLayout(this);

    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        SaveGLViewport();
        SetViewportForOpenScene();
        UILayoutManager::RebuildLayout(openScene);
        LoadGLViewport();
    }

    Texture2D *openSceneTex = nullptr;
    if (openScene)
    {
        Camera *cam = openScene->GetCamera();
        if (cam)
        {
            GBuffer *gbuffer =  cam->GetGBuffer();
            openSceneTex = gbuffer->GetAttachmentTexture(GBuffer::AttColor);
        }
    }
    m_sceneContainer->SetSceneImageTexture(openSceneTex);

    GEngine *gEngine = GEngine::GetInstance();
    RenderOpenScene();
    gEngine->Render(this);
    window->BlitToScreen(GetCamera());
}

EditorScene *EditorScene::GetInstance()
{
    return SCAST<EditorScene*>( SceneManager::GetRootScene() );
}

Console *EditorScene::GetConsole() const { return m_console; }
Explorer *EditorScene::GetExplorer() const { return m_explorer; }
Inspector *EditorScene::GetInspector() const { return m_inspector; }
Hierarchy *EditorScene::GetHierarchy() const { return m_hierarchy; }

void EditorScene::SaveGLViewport()
{
    m_prevGLViewport = GL::GetViewportRect();
}

void EditorScene::LoadGLViewport()
{
    GL::SetViewport(m_prevGLViewport);
}
