#include "BangEditor/EditorApplication.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Window.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Texture2D.h"
#include "Bang/SceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication(int argc, char **argv,
                                     const Path &engineRootPath)
    : Application(argc, argv, engineRootPath)
{
}

EditorApplication::~EditorApplication()
{
    delete m_openSceneRenderTexture;
}

void EditorApplication::CreateWindow()
{
    Application::CreateWindow();
    m_openSceneRenderTexture = new Texture2D();
}

#include "Bang/Time.h"
#include "Bang/UIMask.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
int EditorApplication::MainLoop()
{
    m_openScene = SceneManager::GetActiveScene()->Clone();
    m_openScene->SetCamera( m_openScene->GetComponentInChildren<Camera>() );

    Scene *edScene = new Scene();
    edScene->AddComponent<Transform>();
    Camera *cam = edScene->AddComponent<Camera>();
    cam->SetClearColor(Color::Green);

    UIGameObject *imgMaskGo = GameObjectFactory::CreateUIGameObject();
    UIMask *mask = imgMaskGo->AddComponent<UIMask>();
    mask->SetMasking(false);
    mask->SetDrawMask(true);
    RectTransform *imgMaskRT = imgMaskGo->GetComponent<RectTransform>();
    imgMaskRT->SetMargins(100);
    UIImageRenderer *maskBg = imgMaskGo->AddComponent<UIImageRenderer>();
    maskBg->SetTint(Color::Gray);
    UIBorderRect *border = imgMaskGo->AddComponent<UIBorderRect>();
    border->SetLineColor(Color::Red);
    border->SetLineWidth(5.0f);
    imgMaskGo->SetParent(edScene);

    UIGameObject *openSceneImgGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *openSceneImgRT = openSceneImgGo->GetComponent<RectTransform>();
    // openSceneImgRT->SetAnchors(Vector2::Zero, Vector2::Zero);
    // openSceneImgRT->SetMargins(-100);
    m_openSceneImage = openSceneImgGo->AddComponent<UIImageRenderer>();
    m_openSceneImage->SetAspectRatioMode(AspectRatioMode::Keep);
    m_openSceneImage->SetAspectRatioMode(AspectRatioMode::Ignore);
    openSceneImgGo->SetParent(imgMaskGo);

    SceneManager::LoadScene(edScene);

    OnResize(Window::GetWidthS(), Window::GetHeightS());
    return Application::MainLoop();
}

bool EditorApplication::MainLoopIteration()
{
    m_openSceneImage->SetImageTexture(m_openSceneRenderTexture);
    bool quit = Application::MainLoopIteration();
    if (!quit)
    {
        GEngine *ge = GEngine::GetInstance();
        ge->RenderToScreen(GetWindow()->GetScreenRenderTexture());

        Debug_Log(Time::GetDeltaTime());
    }
    return quit;
}

void EditorApplication::UpdateScene()
{
    Application::UpdateScene();
    if (m_openScene)
    {
        m_openScene->Update();
    }
}

void EditorApplication::RenderScene()
{
    if (m_openScene)
    {
        Camera *openSceneCam = m_openScene->GetCamera();
        openSceneCam->SetRenderTexture(m_openSceneRenderTexture);
        GetGEngine()->Render(m_openScene);
    }

    Application::RenderScene(); // Renders Editor Scene
}

void EditorApplication::OnResize(int newWidth, int newHeight)
{
    Application::OnResize(newWidth, newHeight);
    if (m_openScene)
    {
        RectTransform *openSceneImageRT = m_openSceneImage->gameObject->
                                                GetComponent<RectTransform>();
        Recti openSceneImageRecti = openSceneImageRT->GetScreenSpaceRectPx();
        m_openSceneRenderTexture->Resize(openSceneImageRecti.GetWidth(),
                                         openSceneImageRecti.GetHeight());
        m_openScene->ParentSizeChanged();
    }
}
