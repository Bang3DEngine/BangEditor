#include "BangEditor/ScaleGizmoAxis.h"

#include "Bang/Ray.h"
#include "Bang/Mesh.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Selection.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/AxisFunctions.h"
#include "Bang/DebugRenderer.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ScaleGizmoAxis::ScaleGizmoAxis()
{
    SetName("ScaleGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetRenderPass(RenderPass::Gizmos);

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    p_meshRenderer->SetRenderPass(RenderPass::Gizmos);
    p_meshRenderer->GetMaterial()->SetReceivesLighting(false);
    p_meshRenderer->SetMesh( MeshFactory::GetCube().Get() );
    p_arrowCap->GetTransform()->SetLocalScale( Vector3(0.3f) );

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetRenderPass(RenderPass::Gizmos);
    p_selectionRenderer->SetMesh( MeshFactory::GetCube().Get() );

    p_arrowCap->SetParent(this);
    p_selectionGo->SetParent(this);
}

ScaleGizmoAxis::~ScaleGizmoAxis()
{
}

void ScaleGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (IsBeingGrabbed())
    {
        // Move along axis.
        // First. Find the plane parallel to the axes, and which faces the
        // camera the most.
        Camera *cam = Camera::GetActive();
        Transform *camT = cam->GetGameObject()->GetTransform();
        Vector3 planeNormal = Vector3::Cross( GetAxisVectorWorld(),
                                Vector3::Cross(camT->GetForward(),
                                               GetAxisVectorWorld())).Normalized();
        Vector3 refGoCenter = GetReferencedGameObject()->GetTransform()->GetPosition();

        // Then cast a ray through the mouse position, and see where it intersects
        // with this plane.
        Ray mouseRay = Camera::GetActive()->
                       FromViewportPointNDCToRay( Input::GetMousePositionNDC() );

        bool intersected;
        Vector3 intersection;
        mouseRay.GetIntersectionWithPlane(refGoCenter, planeNormal,
                                          &intersected, &intersection);

        // Then, move the object to the intersection
        if (intersected)
        {
            GameObject *refGo = GetReferencedGameObject();
            Transform *refGoT = refGo->GetTransform();

            Vector3 centerToMousePointV = (intersection - refGoCenter);
            Vector3 centerToMousePointProjV =
                centerToMousePointV.ProjectedOnVector(GetAxisVectorWorld());

            if (GrabHasJustChanged())
            {
                m_startGrabLocalScale = refGoT->GetLocalScale();
                m_startGrabCenterToMousePointProjV = centerToMousePointProjV;
            }
            else
            {
                Vector3 displacementV = centerToMousePointProjV -
                                        m_startGrabCenterToMousePointProjV;

                const int i = GetAxisIndex(GetAxis());
                float displacement = displacementV[i];
                float scaleFactor = (displacement / GetAxisWorldLength()) * 0.5f;
                if (GetAxis() == Axis3D::Z) { scaleFactor *= -1; }
                scaleFactor += 1.0f;

                Vector3 newScale = m_startGrabLocalScale;
                newScale[i] *= (scaleFactor);
                refGoT->SetLocalScale(newScale);

                UpdatePoints(scaleFactor); // Update gizmo length
            }
        }
    }
    else
    {
        UpdatePoints(1.0f);
    }
}

void ScaleGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionGo->SetEnabled(selection);

    TransformGizmoAxis::Render(renderPass, renderChildren);
}

void ScaleGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdatePoints(1.0f);
}

void ScaleGizmoAxis::UpdatePoints(float length)
{
    Vector3 axisFwd = GetAxisVectorLocal() * length;
    p_lineRenderer->SetPoints( {Vector3::Zero, axisFwd} );
    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);

    constexpr float baseScale = 0.4f;
    p_selectionGo->GetTransform()->SetLocalScale( Vector3(baseScale) +
                                                  Vector3::Abs(axisFwd));
    p_selectionGo->GetTransform()->SetLocalPosition( axisFwd *
                                                     (1.0f + baseScale) * 0.5f);
}

void ScaleGizmoAxis::SetColor(const Color &color)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(color);
    p_meshRenderer->GetMaterial()->SetDiffuseColor(color);
}

float ScaleGizmoAxis::GetAxisWorldLength() const
{
    return GetTransform()->
           FromLocalToWorldDirection( GetAxisVectorLocal() ).
           Length();
}
