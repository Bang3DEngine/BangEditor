#include "BangEditor/TranslateGizmoAxis.h"

#include "Bang/Ray.h"
#include "Bang/Mesh.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Selection.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/AxisFunctions.h"
#include "Bang/DebugRenderer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TranslateGizmoAxis::TranslateGizmoAxis()
{
    SetName("TranslateGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetRenderPass(RenderPass::Gizmos);
    p_lineRenderer->SetLineWidth(2.0f);

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    p_meshRenderer->SetRenderPass(RenderPass::Gizmos);
    p_meshRenderer->GetMaterial()->SetReceivesLighting(false);
    p_meshRenderer->SetMesh( MeshFactory::GetCone().Get() );
    p_arrowCap->GetTransform()->SetLocalScale( Vector3(0.5f, 0.5f, 1.0f) );

    p_arrowCap->SetParent(this);
}

TranslateGizmoAxis::~TranslateGizmoAxis()
{
}

void TranslateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (IsBeingGrabbed())
    {
        // Move along axis.
        // First. Find the plane parallel to the axes, and which faces the
        // camera the most.
        Camera *cam = Camera::GetActive();
        Transform *camT = cam->GetGameObject()->GetTransform();
        Vector3 planeNormal = Vector3::Cross( GetAxisVectorLocal(),
                                Vector3::Cross(camT->GetForward(),
                                                GetAxisVectorLocal())).Normalized();
        Vector3 refPos = GetReferencedGameObject()->GetTransform()->GetPosition();

        // Then cast a ray through the mouse position, and see where it intersects
        // with this plane.
        Ray ray = Camera::GetActive()->
                  FromViewportPointNDCToRay( Input::GetMousePositionNDC() );
        Vector3 inters = ray.GetIntersectionWithPlane(planeNormal, refPos);

        // Then, move the object to the intersection
        GameObject *refGo = GetReferencedGameObject();
        Transform *refGoT = refGo->GetTransform();

        Vector3 displacement = (inters - refGoT->GetPosition());
        displacement *= Vector3::Abs( GetAxisVectorWorld() );
        if (GrabHasJustChanged()) { m_grabOffset = displacement; }

        refGoT->Translate(displacement);
        refGoT->Translate(-m_grabOffset);
    }
}

void TranslateGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
    Vector3 axisFwd = GetAxisVectorLocal();
    p_lineRenderer->SetPoints( {Vector3::Zero, axisFwd} );

    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);
    Vector3 axisUp = (axis != Axis3D::Y) ? Vector3::Up : Vector3::Right;
    p_arrowCap->GetTransform()->SetLocalRotation(
           Quaternion::LookDirection(axisFwd, axisUp) );
}

void TranslateGizmoAxis::SetColor(const Color &color)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(color);
    p_meshRenderer->GetMaterial()->SetDiffuseColor(color);
}

