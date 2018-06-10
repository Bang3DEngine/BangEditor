#include "BangEditor/ComponentsGizmos.h"

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
#include "Bang/TextureFactory.h"
#include "Bang/DirectionalLight.h"

#include "BangEditor/Selection.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentsGizmos::ComponentsGizmos()
{
    SetName("ComponentsGizmos");

    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DONT_SERIALIZE);
    GetHideFlags().SetOn(HideFlag::DONT_CLONE);
}

ComponentsGizmos::~ComponentsGizmos()
{
}

void ComponentsGizmos::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (rp == RenderPass::OVERLAY)
    {
        if (GameObject *selectedGameObject = GetReferencedGameObject())
        {
            for (Component *comp : GetReferencedGameObject()->GetComponents())
            {
                if (!comp->IsActive()) { continue; }

                if (!Selection::IsBeingRendered())
                {
                    RenderComponentGizmos(comp, true);
                }
            }
        }

        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            RenderGameObjectGizmosWhenNotSelected(openScene);
        }
    }
}

void ComponentsGizmos::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    Update();
}

void ComponentsGizmos::RenderGameObjectGizmosWhenNotSelected(GameObject *go)
{
    const List<Component*> &comps = go->GetComponents();
    for (Component *comp : comps)
    {
        RenderComponentGizmos(comp, false);
    }

    const List<GameObject*> &goChildren = go->GetChildren();
    for (GameObject *child : goChildren)
    {
        RenderGameObjectGizmosWhenNotSelected(child);
    }
}

void ComponentsGizmos::RenderComponentGizmos(Component *comp,
                                             bool whenCompIsSelected)
{
    SelectionFramebuffer *sfb = Selection::GetSelectionFramebuffer();
    if (!whenCompIsSelected && Selection::IsBeingRendered())
    {
        sfb->SetNextRenderSelectable( comp->GetGameObject() );
    }

    if (Camera *cam = DCAST<Camera*>(comp))
    {
        RenderCameraGizmo(cam, whenCompIsSelected);
    }

    if (PointLight *pl = DCAST<PointLight*>(comp))
    {
        RenderPointLightGizmo(pl, whenCompIsSelected);
    }

    if (DirectionalLight *dl = DCAST<DirectionalLight*>(comp))
    {
        RenderDirectionalLightGizmo(dl, whenCompIsSelected);
    }

    if (AudioSource *as = DCAST<AudioSource*>(comp))
    {
        RenderAudioSourceGizmo(as, whenCompIsSelected);
    }

    if (!whenCompIsSelected && Selection::IsBeingRendered())
    {
        sfb->SetNextRenderSelectable(nullptr);
    }
}

void ComponentsGizmos::RenderCameraGizmo(Camera *cam,
                                         bool whenCompIsSelected)
{
    Transform *camTransform = cam->GetGameObject()->GetTransform();

    GBuffer *gb = GEngine::GetActiveGBuffer();

    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    if (!whenCompIsSelected)
    {
        Gizmos::Reset();
        Gizmos::SetRenderPass(RenderPass::OVERLAY);

        static RH<Mesh> cameraMesh = MeshFactory::GetCamera();
        Transform *camTransform = cam->GetGameObject()->GetTransform();
        float distScale = 15.0f;

        /*
        Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
        Transform *sceneCamTransform = sceneCam->GetGameObject()->GetTransform();
        float distScale = Vector3::Distance(sceneCamTransform->GetPosition(),
                                            camTransform->GetPosition());
        */

        Gizmos::SetReceivesLighting(true);
        Gizmos::SetPosition(camTransform->GetPosition());
        Gizmos::SetRotation(camTransform->GetRotation());
        Gizmos::SetScale(Vector3::One * 0.02f * distScale);
        Gizmos::SetColor(Color::White);
        Gizmos::RenderCustomMesh(cameraMesh.Get());
    }
    else
    {
        Gizmos::Reset();
        Gizmos::SetColor(Color::Green);
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetRenderPass(RenderPass::OVERLAY);

        if (cam->GetProjectionMode() == Camera::ProjectionMode::PERSPECTIVE)
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
            Vector2 orthoSize = Vector2(cam->GetOrthoWidth(),
                                        cam->GetOrthoHeight());
            orthoBox.SetMin(pos + Vector3(-orthoSize.x, -orthoSize.y,
                                          -cam->GetZNear()));
            orthoBox.SetMax(pos + Vector3( orthoSize.x,  orthoSize.y,
                                           -cam->GetZFar()));
            Gizmos::SetRotation(camTransform->GetRotation());
            Gizmos::RenderSimpleBox(orthoBox);
        }
    }

    gb->PopDepthStencilTexture();
}

void ComponentsGizmos::RenderPointLightGizmo(PointLight *pointLight,
                                             bool whenCompIsSelected)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();

    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    if (!whenCompIsSelected)
    {
        Gizmos::Reset();
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        Gizmos::SetColor(pointLight->GetColor().WithAlpha(1.0f));
        Gizmos::SetPosition( pointLight->GetGameObject()->GetTransform()->
                             GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( TextureFactory::GetLightBulbIcon().Get(), true );
    }
    else
    {
        Gizmos::Reset();
        Gizmos::SetThickness(2.0f);
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetColor(pointLight->GetColor());
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        Transform *plTransform = pointLight->GetGameObject()->GetTransform();
        Gizmos::RenderSimpleSphere(plTransform->GetPosition(),
                                   pointLight->GetRange(),
                                   true,
                                   1, 2, 32);
    }

    gb->PopDepthStencilTexture();
}

void ComponentsGizmos::RenderDirectionalLightGizmo(DirectionalLight *dirLight,
                                                   bool whenCompIsSelected)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();
    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    if (!whenCompIsSelected)
    {
        Gizmos::Reset();
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        Gizmos::SetColor( dirLight->GetColor().WithAlpha(1.0f) );
        Gizmos::SetPosition( dirLight->GetGameObject()->GetTransform()->
                             GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( TextureFactory::GetSunIcon().Get(), true );
    }
    else
    {
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
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        for (float ang = 0.0f; ang <= 2 * Math::Pi; ang += Math::Pi / 4.0f)
        {
            const Vector3 offx = right * Math::Cos(ang);
            const Vector3 offy = up * Math::Sin(ang);
            Gizmos::RenderLine(center + offx + offy,
                               center + offx + offy + forward);
        }
    }

    gb->PopDepthStencilTexture();
}

void ComponentsGizmos::RenderAudioSourceGizmo(AudioSource *audioSource,
                                              bool whenCompIsSelected)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();
    gb->PushDepthStencilTexture();
    gb->SetSceneDepthStencil();

    if (!whenCompIsSelected)
    {
        Gizmos::Reset();
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        Gizmos::SetPosition( audioSource->GetGameObject()->
                             GetTransform()->GetPosition() );
        Gizmos::SetScale( Vector3(0.1f) );
        Gizmos::RenderIcon( TextureFactory::GetAudioIcon().Get(), true );
    }
    else
    {
        Gizmos::Reset();
        Gizmos::SetThickness(2.0f);
        Gizmos::SetColor(Color::White);
        Gizmos::SetReceivesLighting(false);
        Gizmos::SetRenderPass(RenderPass::OVERLAY);
        Gizmos::RenderSimpleSphere(audioSource->GetGameObject()->
                                        GetTransform()->GetPosition(),
                                   audioSource->GetRange(),
                                   true,
                                   1, 2, 32);
    }

    gb->PopDepthStencilTexture();
}
