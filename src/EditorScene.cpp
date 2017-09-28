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
#include "Bang/UILayoutManager.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/MenuBar.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/UISceneContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
    AddComponent<RectTransform>();
    AddComponent<UICanvas>();

    m_menuBar = new MenuBar();
    m_menuBar->transform->TranslateLocal( Vector3(0,0,-1) );
    m_menuBar->SetParent(this);

    UIGameObject *vlGo = GameObjectFactory::CreateUIGameObject();
    vlGo->GetRectTransform()->AddMarginTop(MenuBar::GetFixedHeight());
    UIVerticalLayout *vl = vlGo->AddComponent<UIVerticalLayout>();
    vl->SetChildrenHorizontalStretch(Stretch::Full);
    vlGo->SetParent(this);

    GameObject *hlGo = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *hl = hlGo->AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::Full);
    UILayoutElement *hlLe = hlGo->AddComponent<UILayoutElement>();
    hlLe->SetPreferredSize( Vector2i(1, 1) );
    hlLe->SetFlexibleSize( Vector2(999999) );
    hlGo->SetParent(vlGo);

    UIGameObject *overSceneCont = GameObjectFactory::CreateUIGameObject();
    m_sceneContainer = overSceneCont->AddComponent<UISceneContainer>();
    m_sceneContainer->SetPaddings(10);
    overSceneCont->SetParent(hlGo);

    UILayoutElement *fle = overSceneCont->AddComponent<UILayoutElement>();
    fle->SetFlexibleSize( Vector2(99999) );

    m_sceneContainerGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *fle2 = m_sceneContainerGo->AddComponent<UILayoutElement>();
    fle2->SetFlexibleSize( Vector2(99999) );
    m_sceneContainerGo->SetParent(overSceneCont);

    Inspector *inspector = new Inspector();
    inspector->SetParent(hlGo);

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
