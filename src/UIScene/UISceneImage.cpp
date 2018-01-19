#include "BangEditor/UISceneImage.h"

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
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/UISceneDebugStats.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

UISceneImage::UISceneImage()
{
    SetName("SceneContainer");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(170, 100) );
    le->SetFlexibleSize( Vector2(6, 1) );

    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg  = sceneImgGo->AddComponent<UISceneImageRenderer>();
    p_sceneImg->SetUvMultiply(Vector2(1, -1));
    UILayoutElement *imgLE = sceneImgGo->AddComponent<UILayoutElement>();
    imgLE->SetFlexibleSize( Vector2(1) );

    p_sceneDebugStats = GameObject::Create<UISceneDebugStats>();

    sceneImgGo->SetParent(this);
    p_sceneDebugStats->SetParent(this);

    SetShowDebugStats(false);
}

UISceneImage::~UISceneImage()
{

}

void UISceneImage::Update()
{
    GameObject::Update();
}

void UISceneImage::SetSceneImageCamera(Camera *sceneCam)
{
    Texture2D *camTexture = nullptr;
    p_currentCamera = sceneCam;
    if (sceneCam)
    {
        GBuffer *gbuffer =  sceneCam->GetGBuffer();
        switch (GetRenderMode())
        {
            case RenderMode::Color:
            camTexture = gbuffer->GetAttachmentTexture(GBuffer::AttColor);
            break;

            case RenderMode::Normal:
            camTexture = gbuffer->GetAttachmentTexture(GBuffer::AttNormal);
            break;

            case RenderMode::Diffuse:
            camTexture = gbuffer->GetAttachmentTexture(GBuffer::AttDiffuse);
            break;

            case RenderMode::Depth:
            camTexture = gbuffer->GetAttachmentTexture(GBuffer::AttDepthStencil);
            break;

            case RenderMode::Selection:
            {
                SelectionFramebuffer *sfb = sceneCam->GetSelectionFramebuffer();
                camTexture = sfb->GetAttachmentTexture(SelectionFramebuffer::AttColor);
            }
            break;
        }
    }
    p_sceneImg->SetImageTexture(camTexture);

    if (camTexture) { camTexture->SetWrapMode(GL::WrapMode::Repeat); }
    p_sceneImg->SetTint(camTexture ? Color::White : Color::Black);
}

void UISceneImage::SetRenderMode(UISceneImage::RenderMode renderMode)
{
    if (renderMode != GetRenderMode())
    {
        m_renderMode = renderMode;
        SetSceneImageCamera( p_currentCamera );
    }
}

void UISceneImage::SetShowDebugStats(bool showDebugStats)
{
    p_sceneDebugStats->SetVisible( showDebugStats );
}

UISceneImage::RenderMode UISceneImage::GetRenderMode() const
{
    return m_renderMode;
}

void UISceneImage::UISceneImageRenderer::OnRender()
{
    const bool wasBlendEnabled = GL::IsEnabled(GL::Test::Blend);
    GL::Disable(GL::Test::Blend);

    UIImageRenderer::OnRender();

    GL::SetEnabled(GL::Test::Blend, wasBlendEnabled);
}
