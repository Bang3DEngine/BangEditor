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
#include "Bang/RendererFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/TransformGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

float ScaleGizmoAxis::ArrowCapScale = 0.3f;

ScaleGizmoAxis::ScaleGizmoAxis()
{
    SetName("ScaleGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_lineRenderer);

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_meshRenderer);
    p_meshRenderer->GetMaterial()->SetReceivesLighting(false);
    p_meshRenderer->SetMesh( MeshFactory::GetCube().Get() );
    p_arrowCap->GetTransform()->SetLocalScale( Vector3( ArrowCapScale ) );

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_selectionRenderer);
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

            Vector3 centerToMousePointProjLocalV =
                    refGoT->FromWorldToLocalDirection(centerToMousePointProjV);

            if (GrabHasJustChanged())
            {
                m_startGrabLocalScale = refGoT->GetLocalScale();
                m_startGrabCenterToMousePointProjLocalV = centerToMousePointProjLocalV;
            }
            else
            {
                Vector3 displacementLocalV = centerToMousePointProjLocalV -
                                        m_startGrabCenterToMousePointProjLocalV;
                displacementLocalV *= Vector3(1, 1,-1);
                Vector3 scaleFactor = Vector3::Abs(GetAxisVectorLocal()) *
                                       displacementLocalV * 0.5f;
                scaleFactor += 1.0f;

                Vector3 newScale = m_startGrabLocalScale * scaleFactor;
                refGoT->SetLocalScale(newScale);

                // Update gizmo length
                const int i = GetAxisIndex(GetAxis());
                Vector3 centerToMousePointProjLocalGizmoV =
                   GetTransform()->FromWorldToLocalVector(centerToMousePointProjV);
                centerToMousePointProjLocalGizmoV *= Vector3(1, 1,-1);
                UpdatePoints( centerToMousePointProjLocalGizmoV[i] );
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

void ScaleGizmoAxis::UpdatePoints(float localAxisLength)
{
    if (!GetParent()) { return; }

    Vector3 axisFwd = GetAxisVectorLocal() * localAxisLength;
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
