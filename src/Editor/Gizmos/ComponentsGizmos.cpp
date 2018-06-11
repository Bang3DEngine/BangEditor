#include "BangEditor/ComponentsGizmos.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/PointLight.h"
#include "Bang/AudioSource.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"
#include "Bang/RenderFactory.h"
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
        if (GameObject *selectedGameObject = Editor::GetSelectedGameObject())
        {
            for (Component *comp : selectedGameObject->GetComponents())
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

    if (!whenCompIsSelected)
    {
        static RH<Mesh> cameraMesh = MeshFactory::GetCamera();
        Transform *camTransform = cam->GetGameObject()->GetTransform();
        float distScale = 15.0f;
        /*
        Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
        Transform *sceneCamTransform = sceneCam->GetGameObject()->GetTransform();
        float distScale = Vector3::Distance(sceneCamTransform->GetPosition(),
                                            camTransform->GetPosition());
        */

        RenderFactory::Parameters params;
        params.receivesLighting = true;
        params.position = camTransform->GetPosition();
        params.rotation = camTransform->GetRotation();
        params.scale = Vector3(0.02f * distScale);
        params.color = Color::White;
        GL::Disable(GL::Enablable::CULL_FACE);
        GL::Disable(GL::Enablable::DEPTH_TEST);
        RenderFactory::RenderCustomMesh(cameraMesh.Get(), params);
        GL::Enable(GL::Enablable::DEPTH_TEST);
        GL::Enable(GL::Enablable::CULL_FACE);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        RenderFactory::Parameters params;
        params.color = Color::Green;
        params.receivesLighting = false;

        if (cam->GetProjectionMode() == Camera::ProjectionMode::PERSPECTIVE)
        {
            RenderFactory::RenderFrustum(camTransform->GetForward(),
                                         camTransform->GetUp(),
                                         camTransform->GetPosition(),
                                         cam->GetZNear(),
                                         cam->GetZFar(),
                                         cam->GetFovDegrees(),
                                         GL::GetViewportAspectRatio(),
                                         params);
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
            params.rotation = camTransform->GetRotation();
            RenderFactory::RenderSimpleBox(orthoBox, params);
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderPointLightGizmo(PointLight *pointLight,
                                             bool whenCompIsSelected)
{
    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.color = pointLight->GetColor().WithAlpha(1.0f);
    params.position = pointLight->GetGameObject()->
                      GetTransform()->GetPosition();

    if (!whenCompIsSelected)
    {
        params.scale = Vector3(0.1f);
        RenderFactory::RenderIcon(TextureFactory::GetLightBulbIcon().Get(),
                                  true,
                                  params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        params.thickness = 2.0f;
        RenderFactory::RenderSimpleSphere(pointLight->GetRange(),
                                          true,
                                          params,
                                          1, 2, 32);

        gb->PopDepthStencilTexture();
    }

}

void ComponentsGizmos::RenderDirectionalLightGizmo(DirectionalLight *dirLight,
                                                   bool whenCompIsSelected)
{
    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.color = dirLight->GetColor().WithAlpha(1.0f);

    if (!whenCompIsSelected)
    {
        params.position = dirLight->GetGameObject()->
                          GetTransform()->GetPosition();
        params.scale = Vector3(0.1f);
        RenderFactory::RenderIcon(TextureFactory::GetSunIcon().Get(),
                                  true,
                                  params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

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

        params.thickness = 2.0f;
        for (float ang = 0.0f; ang <= 2 * Math::Pi; ang += Math::Pi / 4.0f)
        {
            const Vector3 offx = right * Math::Cos(ang);
            const Vector3 offy = up * Math::Sin(ang);
            RenderFactory::RenderLine(center + offx + offy,
                                      center + offx + offy + forward,
                                      params);
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderAudioSourceGizmo(AudioSource *audioSource,
                                              bool whenCompIsSelected)
{
    RenderFactory::Parameters params;
    params.color = Color::White;
    params.receivesLighting = false;
    params.position = audioSource->GetGameObject()->
                      GetTransform()->GetPosition();

    if (!whenCompIsSelected)
    {
        params.scale = Vector3(0.1f);
        RenderFactory::RenderIcon(TextureFactory::GetAudioIcon().Get(),
                                  true,
                                  params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        params.thickness = 2.0f;
        RenderFactory::RenderSimpleSphere(audioSource->GetRange(),
                                          true,
                                          params,
                                          1, 2, 32);

        gb->PopDepthStencilTexture();
    }
}
