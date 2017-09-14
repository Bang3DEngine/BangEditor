#include "BangEditor/EditorScene.h"

#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorScene::EditorScene()
{
    AddComponent<Transform>();
    Camera *cam = AddComponent<Camera>();
    cam->SetClearColor(Color::Gray);

    UIGameObject *imgMaskGo = GameObjectFactory::CreateUIGameObject();
    UIMask *mask = imgMaskGo->AddComponent<UIMask>();
    mask->SetMasking(false);
    mask->SetDrawMask(true);
    RectTransform *imgMaskRT = imgMaskGo->GetComponent<RectTransform>();
    imgMaskRT->SetMargins(100);
    UIImageRenderer *maskBg = imgMaskGo->AddComponent<UIImageRenderer>();
    maskBg->SetTint(Color::Black);
    UIBorderRect *border = imgMaskGo->AddComponent<UIBorderRect>();
    border->SetLineColor(Color::Red);
    border->SetLineWidth(5.0f);
    imgMaskGo->SetParent(this);

    m_openSceneContainer = GameObjectFactory::CreateUIGameObject();
    m_openSceneImage = m_openSceneContainer->AddComponent<UIImageRenderer>();
    m_openSceneContainer->SetParent(imgMaskGo);

    m_openSceneRenderTexture = new Texture2D();
}

EditorScene::~EditorScene()
{
    delete m_openSceneRenderTexture;
}

void EditorScene::Update()
{
    Scene::Update();
    if (p_openScene)
    {
        p_openScene->Update();
    }
}

void EditorScene::ParentSizeChanged()
{
    Scene::ParentSizeChanged();
    if (p_openScene)
    {
        RectTransform *openSceneImageRT = m_openSceneContainer->
                                                GetComponent<RectTransform>();
        Recti openSceneImageRecti = openSceneImageRT->GetScreenSpaceRectPx();
        m_openSceneRenderTexture->Resize(openSceneImageRecti.GetWidth(),
                                         openSceneImageRecti.GetHeight());
        p_openScene->ParentSizeChanged();
    }
}

void EditorScene::SetOpenScene(Scene *openScene)
{
    p_openScene = openScene;
    if (p_openScene)
    {
        p_openScene->SetFirstFoundCameraOrDefaultOne();
        ParentSizeChanged();
    }
}

Scene *EditorScene::GetOpenScene() const
{
    return p_openScene;
}

void EditorScene::RenderEditor()
{
    if (p_openScene) // First render the open scene
    {
        Camera *openSceneCam = p_openScene->GetCamera();
        if (openSceneCam)
        {
            GEngine::GetInstance()->Render(p_openScene);
            openSceneCam->SetRenderTexture(m_openSceneRenderTexture);
        }
    }
    m_openSceneImage->SetImageTexture(m_openSceneRenderTexture);
    GEngine::GetInstance()->Render(this);
}
