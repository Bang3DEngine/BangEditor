#include "BangEditor/UISceneImage.h"

#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/EditorPaths.h"
#include "BangEditor/UISceneDebugStats.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG_EDITOR

UISceneImage::UISceneImage()
{
    SetName("SceneImage");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(170, 100) );
    le->SetFlexibleSize( Vector2(6, 1) );

    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>(); (void)(vl);

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg  = sceneImgGo->AddComponent<UISceneImageRenderer>();
    p_sceneImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    p_sceneImg->GetMaterial()->SetShaderProgram(
        ShaderProgramFactory::Get(
                        EPATH("Shaders/UIImageRenderer.vert"),
                        EditorPaths::GetEditorAssetsDir().
                            Append("Shaders").Append("UISceneImage.frag")));

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

void UISceneImage::Render(RenderPass renderPass, bool renderChildren)
{
    GameObject::Render(renderPass, renderChildren);

    switch (GetRenderMode())
    {
        case UISceneImage::RenderMode::Depth:
        case UISceneImage::RenderMode::Selection:
        {
            Camera *sceneCam = GetCamera();
            ShaderProgram *sp = p_sceneImg->GetMaterial()->GetShaderProgram();
            switch (GetRenderMode())
            {
                case UISceneImage::RenderMode::Depth:
                {
                GBuffer *gb = sceneCam ? sceneCam->GetGBuffer() : nullptr;
                if (gb && sp)
                {
                    Texture2D *depthTex = gb->GetSceneDepthStencilTexture();
                    sp->SetTexture("B_SceneDepthStencilTex", depthTex);
                }
                }
                break;

                case UISceneImage::RenderMode::Selection:
                {
                SelectionFramebuffer *sfb = sceneCam->GetSelectionFramebuffer();
                if (sfb)
                {
                    Texture2D *selectionTex = sfb->GetColorTexture().Get();
                    sp->SetTexture("B_SelectionTex", selectionTex);
                }
                }
                break;

                default: break;
            }

        }
        break;

        default: break;
    }

}

void UISceneImage::SetSceneImageCamera(Camera *sceneCam)
{
    Texture2D *camTexture = nullptr;
    p_currentCamera = sceneCam;

    if (sceneCam)
    {
        GBuffer *camGBuffer =  sceneCam->GetGBuffer();

        ShaderProgram *sp = p_sceneImg->GetActiveMaterial()->GetShaderProgram();
        GLId prevBoundSP = GL::GetBoundId(GL::BindTarget::ShaderProgram);

        sp->Bind();
        sp->SetInt("B_SceneRenderMode", SCAST<int>(GetRenderMode()), false);
        camGBuffer->BindAttachmentsForReading(sp, false);

        GL::Bind(GL::BindTarget::ShaderProgram, prevBoundSP);
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
        SetSceneImageCamera( GetCamera() );
    }
}

void UISceneImage::SetShowDebugStats(bool showDebugStats)
{
    p_sceneDebugStats->SetVisible( showDebugStats );
}

Camera *UISceneImage::GetCamera() const
{
    return p_currentCamera;
}

UISceneImage::RenderMode UISceneImage::GetRenderMode() const
{
    return m_renderMode;
}

void UISceneImage::UISceneImageRenderer::OnRender()
{
    const bool wasBlendEnabled = GL::IsEnabled(GL::Enablable::Blend);
    GL::Disable(GL::Enablable::Blend, 0);

    UIImageRenderer::OnRender();

    GL::SetEnabled(GL::Enablable::Blend, wasBlendEnabled, false);
}
