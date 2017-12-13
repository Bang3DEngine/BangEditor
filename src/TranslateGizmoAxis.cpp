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
    SetName("TranslateGizmo");

    AddComponent<Transform>();

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
    GameObject::Update();

    // Change color depending on selection state
    // Debug_Peek(Selection::GetOveredGameObject());
    bool isMouseOver = (Selection::GetOveredGameObject() == this ||
                        Selection::GetOveredGameObject() == p_arrowCap);
    SelectionState state = m_beingGrabbed ? SelectionState::Grabbed :
                 (isMouseOver ? SelectionState::Over : SelectionState::Idle);
    SetColor(state);

    // Change being grabbed
    bool prevGrab = m_beingGrabbed;
    if (!m_beingGrabbed)
    {
        m_beingGrabbed = isMouseOver && Input::GetMouseButtonDown(MouseButton::Left);
    }
    else
    {
        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            m_beingGrabbed = false;
        }
    }
    bool grabHasJustChanged = (m_beingGrabbed != prevGrab);

    if (m_beingGrabbed)
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
        if (grabHasJustChanged) { m_grabOffset = displacement; }

        refGoT->Translate(displacement);
        refGoT->Translate(-m_grabOffset);
    }
}

void TranslateGizmoAxis::SetAxis(Axis3D axis)
{
    if (axis != GetAxis())
    {
        m_axis = axis;

        SetColor(SelectionState::Idle);

        Vector3 axisFwd = GetAxisVectorLocal();
        p_lineRenderer->SetPoints( {Vector3::Zero, axisFwd} );

        p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);
        Vector3 axisUp = (axis != Axis3D::Y) ? Vector3::Up : Vector3::Right;
        p_arrowCap->GetTransform()->SetLocalRotation(
               Quaternion::LookDirection(axisFwd, axisUp) );
    }
}
Axis3D TranslateGizmoAxis::GetAxis() const { return m_axis; }
Vector3 TranslateGizmoAxis::GetAxisVectorLocal() const
{
    return GetVectorFromAxis( GetAxis() );
}

Vector3 TranslateGizmoAxis::GetAxisVectorWorld() const
{
    Transform *refGoT = GetReferencedGameObject()->GetTransform();
    return refGoT->FromLocalToWorldDirection( GetAxisVectorLocal() );
}

void TranslateGizmoAxis::SetColor(SelectionState state)
{
    switch (state)
    {
        case SelectionState::Idle:
            {
            Color axisColor = GetColorFromAxis( GetAxis() );
            p_lineRenderer->GetMaterial()->SetDiffuseColor(axisColor);
            p_meshRenderer->GetMaterial()->SetDiffuseColor(axisColor);
            }
            break;

        case SelectionState::Over:
            p_lineRenderer->GetMaterial()->SetDiffuseColor(Color::Orange);
            p_meshRenderer->GetMaterial()->SetDiffuseColor(Color::Orange);
            break;

        case SelectionState::Grabbed:
            p_lineRenderer->GetMaterial()->SetDiffuseColor(Color::Yellow);
            p_meshRenderer->GetMaterial()->SetDiffuseColor(Color::Yellow);
            break;
    }

}
