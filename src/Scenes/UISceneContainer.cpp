#include "BangEditor/UISceneContainer.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/SceneDebugStats.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

UISceneContainer::UISceneContainer()
{
    SetName("SceneContainer");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(100) );
    le->SetFlexibleSize( Vector2(6, 1) );

    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg  = sceneImgGo->AddComponent<UIImageRenderer>();
    p_sceneImg->SetUvMultiply(Vector2(1, -1));
    UILayoutElement *imgLE = sceneImgGo->AddComponent<UILayoutElement>();
    imgLE->SetFlexibleSize( Vector2(1) );

    GameObject *cameraPreviewGo = GameObjectFactory::CreateUIGameObject();
    p_cameraPreviewImg = cameraPreviewGo->AddComponent<UIImageRenderer>();
    p_cameraPreviewImg->SetUvMultiply(Vector2(1, -1));
    p_cameraPreviewImg->SetVisible(false);

    p_sceneDebugStats = GameObject::Create<SceneDebugStats>();

    cameraPreviewGo->GetRectTransform()->SetAnchors( Vector2(0.5f, 0.5f),
                                                     Vector2(1.0f, 1.0f) );

    sceneImgGo->SetParent(this);
    cameraPreviewGo->SetParent(this);
    p_sceneDebugStats->SetParent(this);

    SetShowDebugStats(false);
    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
}

UISceneContainer::~UISceneContainer()
{

}

void UISceneContainer::Update()
{
    GameObject::Update();

    // Camera preview handling
    p_cameraPreviewImg->SetVisible( p_selectedCamera != nullptr );
    if (p_selectedCamera)
    {
        // Get preview texture
        GBuffer *gbuffer = p_selectedCamera->GetGBuffer();
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
        gbuffer->Resize(previewRectPx.GetWidth(), previewRectPx.GetHeight());

        p_cameraPreviewImg->GetGameObject()->GetRectTransform()->SetAnchors(
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMin()),
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMax()));

        gbuffer->Resize(previewRectPx.GetWidth(), previewRectPx.GetHeight());

        // Render
        Recti prevViewport = GL::GetViewportRect();
        GL::SetViewport(previewRectPx);
        Scene *openScene = EditorSceneManager::GetOpenScene();
        GEngine::GetActive()->Render(openScene, p_selectedCamera);
        GL::SetViewport(prevViewport);
    }
}

void UISceneContainer::SetShowDebugStats(bool showDebugStats)
{
    p_sceneDebugStats->SetVisible( showDebugStats );
}

Rect UISceneContainer::GetImageScreenRectNDC() const
{
    return p_sceneImg->GetGameObject()->GetRectTransform()->GetViewportRectNDC();
}

void UISceneContainer::SetSceneImageTexture(Texture2D *texture)
{
    p_sceneImg->SetImageTexture(texture);
    if (texture) { texture->SetWrapMode(GL::WrapMode::Repeat); }
    p_sceneImg->SetTint(texture ? Color::White : Color::Black);
}

void UISceneContainer::OnGameObjectSelected(GameObject *selectedGo)
{
    p_selectedCamera = (selectedGo ? selectedGo->GetComponent<Camera>() : nullptr);
}
