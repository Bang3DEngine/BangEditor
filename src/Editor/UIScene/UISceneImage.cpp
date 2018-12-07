#include "BangEditor/UISceneImage.h"

#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/GBuffer.h"
#include "Bang/GL.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Material.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/ResourceHandle.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Texture2D.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/UISceneDebugStats.h"

using namespace BangEditor;

UISceneImage::UISceneImage()
{
    SetName("SceneImage");

    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleSize(Vector2::One());

    AddComponent<UIImageRenderer>()->SetTint(Color::Black());

    // UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    // BANG_UNUSED(vl);

    const Path engShadersDir = Paths::GetEngineAssetsDir().Append("Shaders");
    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg = sceneImgGo->AddComponent<UISceneImageRenderer>();
    p_sceneImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    p_sceneImg->GetMaterial()->SetShaderProgram(ShaderProgramFactory::Get(
        EPATH("Shaders").Append("UIImageRenderer.vert"),
        EditorPaths::GetEditorAssetsDir().Append("Shaders").Append(
            "UISceneImage.frag")));

    UILayoutElement *imgLE = sceneImgGo->AddComponent<UILayoutElement>();
    imgLE->SetFlexibleSize(Vector2::One());

    p_sceneDebugStats = new UISceneDebugStats();

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

        ShaderProgram *sp = p_sceneImg->GetMaterial()->GetShaderProgram();

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
        SetSceneImageCamera(GetCamera());
    }
}

void UISceneImage::SetShowDebugStats(bool showDebugStats)
{
    p_sceneDebugStats->SetVisible(showDebugStats);
}

Camera *UISceneImage::GetCamera() const
{
    return p_currentCamera;
}

UISceneImage::RenderMode UISceneImage::GetRenderMode() const
{
    return m_renderMode;
}

UISceneImage::UISceneImageRenderer *UISceneImage::GetSceneImageRenderer() const
{
    return p_sceneImg;
}

void UISceneImage::UISceneImageRenderer::OnRender()
{
    GL::Push(GL::Pushable::BLEND_STATES);

    GL::Disable(GL::Enablable::BLEND);

    UIImageRenderer::OnRender();

    GL::Pop(GL::Pushable::BLEND_STATES);
}
