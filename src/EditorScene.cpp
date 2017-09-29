#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
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
    m_menuBar->transform->TranslateLocal( Vector3(0, 0, -0.01) );
    m_menuBar->SetParent(this);

    UIGameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    vlGo->GetRectTransform()->AddMarginTop(MenuBar::GetFixedHeight());
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vlGo->SetParent(this);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *hlLe = hlGo->AddComponent<UILayoutElement>();
    hlLe->SetFlexibleSize( Vector2(999999) );
    hlGo->SetParent(vlGo);

    UIGameObject *overSceneCont = GameObjectFactory::CreateUIGameObject();
    m_sceneContainer = overSceneCont->AddComponent<UISceneContainer>();
    m_sceneContainer->SetPaddings(10);
    overSceneCont->SetParent(hlGo);

    UILayoutElement *fle = overSceneCont->AddComponent<UILayoutElement>();
    fle->SetFlexibleSize( Vector2(99999) );

    m_sceneContainerGo = GameObjectFactory::CreateUIGameObject();
    m_noSceneImg  = m_sceneContainerGo->AddComponent<UIImageRenderer>();
    m_noSceneImg->SetTint(Color::White);

    m_noSceneText = m_sceneContainerGo->AddComponent<UITextRenderer>();
    m_noSceneText->SetContent("Empty Scene");
    m_noSceneText->SetWrapping(true);
    m_noSceneText->SetTextSize(50);

    UILayoutElement *fle2 = m_sceneContainerGo->AddComponent<UILayoutElement>();
    fle2->SetFlexibleSize( Vector2(99999) );
    m_sceneContainerGo->SetParent(overSceneCont);

    m_inspector = new Inspector();
    m_inspector->SetParent(hlGo);

    m_hierarchy = new Hierarchy();
    m_hierarchy->SetParent(hlGo, 0);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *botHLLe = botHLGo->AddComponent<UILayoutElement>();
    botHLLe->SetMinSize( Vector2i(1, 150) );
    botHLLe->SetFlexibleSize( Vector2(999999, 0) );
    botHLGo->SetParent(vlGo);

    m_console = new Console();
    m_console->SetParent(botHLGo);

    m_explorer = new Explorer();
    m_explorer->SetParent(botHLGo);

    Camera *cam = AddComponent<Camera>();
    cam->SetClearColor(Color::Zero);
    SetCamera(cam);
}

EditorScene::~EditorScene()
{
}

void EditorScene::Update()
{
    Scene::Update();
}

void EditorScene::Render(RenderPass rp, bool renderChildren)
{
    Scene::Render(rp, renderChildren);
}

void EditorScene::RenderOpenScene()
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

            Recti prevViewport = GL::GetViewportRect();
            GL::SetViewport(vpRectPx);

            GEngine::GetInstance()->Render(openScene);

            GL::SetViewport(prevViewport);
        }
    }

    m_noSceneImg->SetEnabled (!openScene);
    m_noSceneText->SetEnabled(!openScene);
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
