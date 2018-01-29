#include "BangEditor/ComponentsSelectionGizmo.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
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
    GameObject::Update();
}

void ComponentsSelectionGizmo::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (!GetReferencedGameObject()) { return; }
    for (Component *comp : GetReferencedGameObject()->GetComponents())
    {
        if (Camera *cam = DCAST<Camera*>(comp)) { RenderCameraGizmo(cam, rp); }
    }
}

void ComponentsSelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    Update();
}

void ComponentsSelectionGizmo::RenderCameraGizmo(Camera *cam, RenderPass rp)
{
    if (rp == RenderPass::Scene)
    {
        Transform *camTransform = cam->GetGameObject()->GetTransform();

        Gizmos::Reset();
        Gizmos::SetRenderPass(rp);
        Gizmos::SetColor(Color::Green);
        Gizmos::SetReceivesLighting(false);

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
}
