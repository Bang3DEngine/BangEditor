#include "BangEditor/ComponentsSelectionGizmo.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/PointLight.h"
#include "Bang/AudioSource.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/DirectionalLight.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentsSelectionGizmo::ComponentsSelectionGizmo()
{
    SetName("ComponentsSelectionGizmo");

    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DontSerialize);
    GetHideFlags().SetOn(HideFlag::DontClone);
}

ComponentsSelectionGizmo::~ComponentsSelectionGizmo()
{
}

void ComponentsSelectionGizmo::Update()
{
    SelectionGizmo::Update();
}

void ComponentsSelectionGizmo::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (!GetReferencedGameObject()) { return; }

    for (Component *comp : GetReferencedGameObject()->GetComponents())
    {
        if (!comp->IsActive()) { continue; }

        if (rp == RenderPass::Overlay)
        {
            if (Camera *cam = DCAST<Camera*>(comp))
            { RenderCameraGizmo(cam); }

            if (PointLight *pl = DCAST<PointLight*>(comp))
            { RenderPointLightGizmo(pl); }

            if (DirectionalLight *dl = DCAST<DirectionalLight*>(comp))
            { RenderDirectionalLightGizmo(dl); }

            if (AudioSource *as = DCAST<AudioSource*>(comp))
            { RenderAudioSourceGizmo(as); }
        }
    }
}

void ComponentsSelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    Update();
}

void ComponentsSelectionGizmo::RenderCameraGizmo(Camera *cam)
{
    Transform *camTransform = cam->GetGameObject()->GetTransform();

    GBuffer *gb = GEngine::GetActiveGBuffer();

    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    Gizmos::Reset();
    Gizmos::SetColor(Color::Green);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetRenderPass(RenderPass::Overlay);

    if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
    {
        Gizmos::RenderFrustum(camTransform->GetForward(),
                              camTransform->GetUp(),
                              camTransform->GetPosition(),
                              cam->GetZNear(),
                              cam->GetZFar(),
                              cam->GetFovDegrees(),
                              GL::GetViewportAspectRatio() );
    }
    else
    {
        AABox orthoBox;
        Vector3 pos = camTransform->GetPosition();
        Vector2 orthoSize = Vector2(cam->GetOrthoWidth(), cam->GetOrthoHeight());
        orthoBox.SetMin(pos + Vector3(-orthoSize.x, -orthoSize.y, -cam->GetZNear()));
        orthoBox.SetMax(pos + Vector3( orthoSize.x,  orthoSize.y, -cam->GetZFar()));
        Gizmos::SetRotation(camTransform->GetRotation());
        Gizmos::RenderSimpleBox(orthoBox);
    }

    gb->PopDepthStencilTexture();
}

void ComponentsSelectionGizmo::RenderPointLightGizmo(PointLight *pointLight)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();

    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    Gizmos::Reset();
    Gizmos::SetThickness(2.0f);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetColor(pointLight->GetColor());
    Gizmos::SetRenderPass(RenderPass::Overlay);
    Transform *plTransform = pointLight->GetGameObject()->GetTransform();
    Gizmos::RenderSimpleSphere(plTransform->GetPosition(),
                               pointLight->GetRange(),
                               true,
                               1, 2, 32);

    gb->PopDepthStencilTexture();
}

void ComponentsSelectionGizmo::RenderDirectionalLightGizmo(
                                                    DirectionalLight *dirLight)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();
    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    Gizmos::Reset();

    GameObject *lightGo = dirLight->GetGameObject();
    GameObject *camGo = Camera::GetActive()->GetGameObject();
    float distScale = Vector3::Distance(camGo->GetTransform()->GetPosition(),
                                        lightGo->GetTransform()->GetPosition());
    const float radius = 0.03f * distScale;
    const float length = 0.2f * distScale;
    const Vector3 up = lightGo->GetTransform()->GetUp() * radius;
    const Vector3 right = lightGo->GetTransform()->GetRight() * radius;
    const Vector3 forward = lightGo->GetTransform()->GetForward() * length;
    const Vector3 center = lightGo->GetTransform()->GetPosition();

    Gizmos::SetThickness(2.0f);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetColor(dirLight->GetColor());
    Gizmos::SetRenderPass(RenderPass::Overlay);
    for (float ang = 0.0f; ang <= 2 * Math::Pi; 
         ang += SCAST<float>(Math::Pi / 4.0f) )
    {
        const Vector3 offx = right * Math::Cos(ang);
        const Vector3 offy = up * Math::Sin(ang);
        Gizmos::RenderLine(center + offx + offy, center + offx + offy + forward);
    }

    gb->PopDepthStencilTexture();
}

void ComponentsSelectionGizmo::RenderAudioSourceGizmo(AudioSource *audioSource)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();
    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    Gizmos::Reset();
    Gizmos::SetThickness(2.0f);
    Gizmos::SetColor(Color::White);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetRenderPass(RenderPass::Overlay);
    Gizmos::RenderSimpleSphere(audioSource->GetGameObject()->
                                    GetTransform()->GetPosition(),
                               audioSource->GetRange(),
                               true,
                               1, 2, 32);

    gb->PopDepthStencilTexture();
}
