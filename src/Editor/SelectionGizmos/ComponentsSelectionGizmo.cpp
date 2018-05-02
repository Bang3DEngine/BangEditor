#include "BangEditor/ComponentsSelectionGizmo.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/PointLight.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"

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

        if (rp == RenderPass::Scene)
        {
            if (Camera *cam = DCAST<Camera*>(comp)) { RenderCameraGizmo(cam); }
        }
        else if (rp == RenderPass::Overlay)
        {
            if (PointLight *pl = DCAST<PointLight*>(comp))
            { RenderPointLightGizmo(pl); }
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

    Gizmos::Reset();
    Gizmos::SetColor(Color::Green);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetRenderPass(RenderPass::Scene);

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
}

void ComponentsSelectionGizmo::RenderPointLightGizmo(PointLight *pointLight)
{
    Transform *plTransform = pointLight->GetGameObject()->GetTransform();

    Gizmos::Reset();
    Gizmos::SetThickness(2.0f);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetColor(pointLight->GetColor());
    Gizmos::RenderSimpleSphere(plTransform->GetPosition(),
                               pointLight->GetRange(),
                               true,
                               1, 2, 32);
    /*
    Gizmos::RenderBillboardCircle(plTransform->GetPosition(),
                                  pointLight->GetRange(), 32);

    Gizmos::Reset();
    Gizmos::SetCulling(true);
    Gizmos::SetReceivesLighting(false);
    Gizmos::SetColor(pointLight->GetColor().WithAlpha(0.2f));
    Gizmos::SetRenderPass(RenderPass::Scene);
    Gizmos::RenderSphere(plTransform->GetPosition(), pointLight->GetRange());
    */
}
