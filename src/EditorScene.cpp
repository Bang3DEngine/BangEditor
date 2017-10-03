#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/Window.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UICanvas.h"
#include "Bang/Texture2D.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/UIFrameLayout.h"
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
    AddComponent<RectTransform>();
    AddComponent<UICanvas>();

    m_menuBar = new MenuBar();
    m_menuBar->transform->TranslateLocal( Vector3(0, 0, -0.1) );
    // m_menuBar->SetParent(this);

    m_mainEditorVL = GameObjectFactory::CreateUIGameObject();
    m_mainEditorVL->GetRectTransform()->AddMarginTop(MenuBar::GetFixedHeight());
    UIVerticalLayout *vl = m_mainEditorVL->AddComponent<UIVerticalLayout>();
    m_mainEditorVL->SetParent(this);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *hlLe = hlGo->AddComponent<UILayoutElement>();
    hlGo->SetParent(m_mainEditorVL);

    m_mainEditorVL->AddChild(
              GameObjectFactory::CreateGUIHSeparator(LayoutSizeType::Min, 10));

    UIGameObject *overSceneCont = GameObjectFactory::CreateUIGameObject();
    m_sceneContainer = overSceneCont->AddComponent<UISceneContainer>();
    m_sceneContainer->SetPaddings(10);
    overSceneCont->SetParent(hlGo);

    UILayoutElement *fle = overSceneCont->AddComponent<UILayoutElement>();
    fle->SetFlexibleSize( Vector2(6) );

    m_sceneContainerGo = GameObjectFactory::CreateUIGameObject();
    m_sceneImg  = m_sceneContainerGo->AddComponent<UIImageRenderer>();
    m_sceneImg->SetTint(Color::White);
    m_sceneImg->SetUvMultiply(Vector2(1, -1));

    m_noSceneText = m_sceneContainerGo->AddComponent<UITextRenderer>();
    m_noSceneText->SetContent("Empty Scene");
    m_noSceneText->SetWrapping(true);
    m_noSceneText->SetTextSize(50);

    UILayoutElement *fle2 = m_sceneContainerGo->AddComponent<UILayoutElement>();
    m_sceneContainerGo->SetParent(overSceneCont);

    m_inspector = new Inspector();
    m_inspector->SetParent(hlGo);

    m_hierarchy = new Hierarchy();
    m_hierarchy->SetParent(hlGo, 0);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    botHLLe->SetMinSize( Vector2i(1, 150) );
    botHLGo->SetParent(m_mainEditorVL);

    m_console = new Console();
    m_console->SetParent(botHLGo);

    m_explorer = new Explorer();
    m_explorer->SetParent(botHLGo);

    SetFirstFoundCameraOrDefaultOne();
    // Camera *cam = AddComponent<Camera>();
    GetCamera()->SetClearColor(Color::Yellow);
}

EditorScene::~EditorScene()
{
}

void EditorScene::Update()
{
    Scene::Update();
    if (GetOpenScene()) { GetOpenScene()->Update(); }
}

void EditorScene::RenderOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        Recti prevViewport = GL::GetViewportRect();
        SetViewportForOpenScene();

        GEngine::GetInstance()->Render(openScene);

        GL::SetViewport(prevViewport);
    }

    m_noSceneText->SetEnabled(!openScene);
}

void EditorScene::SetViewportForOpenScene()
{
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        Camera *openSceneCam = openScene->GetCamera();
        if (openSceneCam)
        {
            RectTransform *sceneContRT =
                    m_sceneContainerGo->GetComponent<RectTransform>();
            Rect ndcRect = sceneContRT->GetScreenSpaceRectNDC();

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
        UILayoutManager::ForceRebuildLayout(p_openScene);
        p_openScene->Start();
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

void EditorScene::RenderAndBlitToScreen()
{
    Window *window = Window::GetCurrent();
    window->Clear();

    UILayoutManager::RebuildLayout(this);
    GetUILayoutManager()->OnBeforeRender(this);
    /*
    Scene *openScene = GetOpenScene();
    if (openScene)
    {
        UILayoutManager::RebuildLayout(openScene);
        openScene->GetUILayoutManager()->OnBeforeRender(openScene);
    }
    */

    Texture2D *openSceneTex = nullptr;
    /*
    if (openScene)
    {
        Camera *cam = openScene->GetCamera();
        if (cam)
        {
            GBuffer *gbuffer =  cam->GetGBuffer();
            openSceneTex = gbuffer->GetAttachmentTexture(GBuffer::AttColor);
        }
    }*/
    m_sceneImg->SetImageTexture(openSceneTex);

    GEngine *gEngine = GEngine::GetInstance();
    // RenderOpenScene();
    gEngine->Render(this);
    window->BlitToScreen(GetCamera());
}
