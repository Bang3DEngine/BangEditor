#include "BangEditor/UISceneEditContainer.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/Selection.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/EditorCamera.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneEditContainer::UISceneEditContainer()
{
    ScenePlayer::GetInstance()->
            EventEmitter<IScenePlayerListener>::RegisterListener(this);
    SceneManager::GetActive()->
            EventEmitter<ISceneManagerListener>::RegisterListener(this);

    GameObject *cameraPreviewGo = GameObjectFactory::CreateUIGameObject();
    p_cameraPreviewImg = cameraPreviewGo->AddComponent<UIImageRenderer>();
    p_cameraPreviewImg->GetMaterial()->SetUvMultiply(Vector2(1, -1));
    p_cameraPreviewImg->SetVisible(false);

    cameraPreviewGo->AddComponent<UILayoutIgnorer>();
    cameraPreviewGo->SetParent(this);
}

UISceneEditContainer::~UISceneEditContainer()
{
}

void UISceneEditContainer::Update()
{
    GameObject::Update();

    SetScene( EditorSceneManager::GetOpenScene() );

    GameObject *selectedGO = Editor::GetSelectedGameObject();
    Camera *selectedCamera = selectedGO ? selectedGO->GetComponent<Camera>() :
                                          nullptr;
    // Camera preview handling
    if (selectedCamera)
    {
        // Get preview texture
        GBuffer *gbuffer = selectedCamera->GetGBuffer();
        Texture2D *camTexture = gbuffer->GetAttachmentTexture(GBuffer::AttColor);
        camTexture->SetWrapMode( GL::WrapMode::Repeat );
        p_cameraPreviewImg->SetImageTexture(camTexture);

        // Set preview size
        RectTransform *rt = GetRectTransform();
        Recti sceneContainerRect( rt->GetViewportRect() );
        Vector2i sceneContainerSize = sceneContainerRect.GetSize();

        Vector2i previewRectSize = sceneContainerSize / 4;
        Recti previewRectPx(sceneContainerRect.GetMax(),
                            sceneContainerRect.GetMax() - Vector2i(previewRectSize));

        previewRectPx.SetMin(sceneContainerRect.GetMin());
        previewRectPx.SetMax(previewRectPx.GetMin() + Vector2i(previewRectSize));

        const Vector2i marginsBotLeft = Vector2i(5);
        p_cameraPreviewImg->GetGameObject()->GetRectTransform()->SetAnchors(
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMin() + marginsBotLeft),
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMax()));

        gbuffer->Resize(previewRectPx.GetWidth(), previewRectPx.GetHeight());

        // Render
        Recti prevViewport = GL::GetViewportRect();
        GL::SetViewport(previewRectPx);
        Scene *openScene = EditorSceneManager::GetOpenScene();
        GEngine::GetActive()->Render(openScene, selectedCamera);
        GL::SetViewport(prevViewport);
    }
    p_cameraPreviewImg->SetVisible( selectedCamera != nullptr );
}

void UISceneEditContainer::HandleSelection()
{
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        UICanvas *canvas = UICanvas::GetActive(this);
        bool isOverSceneCont = canvas->IsMouseOver(this, true);
        if (isOverSceneCont)
        {
            Camera *cam = EditorCamera::GetInstance()->GetCamera();
            GameObject *selectedGameObject = Selection::GetOveredGameObject(cam);
            if (selectedGameObject)
            {
                Editor::SelectGameObject(selectedGameObject);
            }
            else { Editor::SelectGameObject(nullptr); }
        }
    }
}

Camera* UISceneEditContainer::GetSceneCamera(Scene *scene)
{
    Camera *editorCamera = EditorCamera::GetInstance()->GetCamera();
    if (editorCamera) { return editorCamera; }
    return scene->GetCamera();
}

bool UISceneEditContainer::NeedsToRenderScene(Scene *scene)
{
    return IsVisible();
}

void UISceneEditContainer::OnPlayStateChanged(PlayState, PlayState)
{
}

void UISceneEditContainer::OnSceneLoaded(Scene *scene, const Path&)
{
}
