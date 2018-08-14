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

#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/UISceneDebugStats.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/SelectionFramebuffer.h"

USING_NAMESPACE_BANG_EDITOR

UISceneImage::UISceneImage()
{
    SetName("SceneImage");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleHeight(1.0f);

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
    Camera *sceneCam = GetCamera();
    GBuffer *gb = sceneCam ? sceneCam->GetGBuffer() : nullptr;
    ShaderProgram *sp = p_sceneImg->GetMaterial()->GetShaderProgram();
    switch (GetRenderMode())
    {
        case UISceneImage::RenderMode::DEPTH:
        case UISceneImage::RenderMode::SELECTION:
        case UISceneImage::RenderMode::WORLD_POSITION:
        {
            switch (GetRenderMode())
            {
                case UISceneImage::RenderMode::DEPTH:
                case UISceneImage::RenderMode::WORLD_POSITION:
                {
                if (gb && sp)
                {
                    Texture2D *depthTex = gb->GetSceneDepthStencilTexture();
                    sp->SetTexture2D("B_SceneDepthStencilTex", depthTex);
                }
                }
                break;

                case UISceneImage::RenderMode::SELECTION:
                {
                EditorCamera *edCam = EditorCamera::GetInstance();
                SelectionFramebuffer *sfb = edCam->GetSelectionFramebuffer();
                if (sfb)
                {
                    Texture2D *selectionTex = sfb->GetColorTexture().Get();
                    sp->SetTexture2D("B_SelectionTex", selectionTex);
                }
                }
                break;

                default: break;
            }

        }
        break;

        default: break;
    }

    if (gb)
    {
        GL::Push(GL::BindTarget::SHADER_PROGRAM);

        sp->Bind();
        gb->BindAttachmentsForReading(sp);

        GL::Pop(GL::BindTarget::SHADER_PROGRAM);
    }

    GameObject::Render(renderPass, renderChildren);

}

void UISceneImage::SetSceneImageCamera(Camera *sceneCam)
{
    p_currentCamera = sceneCam;

    if (sceneCam)
    {
        GBuffer *camGBuffer = sceneCam->GetGBuffer();

        ShaderProgram *sp = p_sceneImg->GetActiveMaterial()->GetShaderProgram();

        GL::Push(GL::BindTarget::SHADER_PROGRAM);

        sp->Bind();
        sp->SetInt("B_SceneRenderMode", SCAST<int>(GetRenderMode()), false);
        camGBuffer->BindAttachmentsForReading(sp);

        GL::Pop(GL::BindTarget::SHADER_PROGRAM);
    }
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
    GL::Push(GL::Pushable::BLEND_STATES);

    GL::Disable(GL::Enablable::BLEND);

    UIImageRenderer::OnRender();

    GL::Pop(GL::Pushable::BLEND_STATES);
}
