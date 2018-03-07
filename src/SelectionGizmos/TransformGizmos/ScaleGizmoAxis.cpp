#include "BangEditor/ScaleGizmoAxis.h"

#include "Bang/GL.h"
#include "Bang/Ray.h"
#include "Bang/Mesh.h"
#include "Bang/Input.h"
#include "Bang/Plane.h"
#include "Bang/Camera.h"
#include "Bang/Sphere.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Geometry.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Selection.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/AxisFunctions.h"
#include "Bang/DebugRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/TransformGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

float ScaleGizmoAxis::ArrowCapScale = 0.3f;

ScaleGizmoAxis::ScaleGizmoAxis()
{
    SetName("ScaleGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetMaterial(MaterialFactory::GetGizmosUnLightedOverlay().Get());

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    p_meshRenderer->SetMaterial(MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_meshRenderer->GetMaterial()->SetReceivesLighting(false);
    p_meshRenderer->SetMesh( MeshFactory::GetCube().Get() );
    p_arrowCap->GetTransform()->SetLocalScale( Vector3( ArrowCapScale ) );

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetMaterial(MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_selectionRenderer->SetMesh( MeshFactory::GetCube().Get() );

    p_arrowCap->SetParent(this);
    p_selectionGo->SetParent(this);
}

ScaleGizmoAxis::~ScaleGizmoAxis()
{
}

void ScaleGizmoAxis::SetReferencedGameObject(GameObject *referencedGameObject)
{
    TransformGizmoAxis::SetReferencedGameObject(referencedGameObject);
    UpdatePoints(1.0f);
}

void ScaleGizmoAxis::Update()
{
    TransformGizmoAxis::Update();
    GameObject *refGo = GetReferencedGameObject();
    if (!refGo || !refGo->GetTransform()) { return; }

    if (IsBeingGrabbed())
    {
        Camera *cam = GetEditorCamera();
        Transform *camT = cam->GetGameObject()->GetTransform();

        Transform *refGoT = refGo->GetTransform();
        Vector3 refGoCenter = refGoT->GetPosition();

        if (GrabHasJustChanged())
        {
            m_startGrabLocalScale = refGoT->GetLocalScale();
        }

        Plane plane;
        plane.SetPoint(refGoCenter);
        if (GetAxis() != Axis3DExt::XYZ)
        {
            // Find the plane parallel to the axes, and which faces the
            // camera the most.
            plane.SetNormal(Vector3::Cross( GetAxisVectorWorld(),
                            Vector3::Cross(camT->GetForward(),
                                           GetAxisVectorWorld())).Normalized());
        }
        else
        {
            // Plane parallel to camera plane
            plane.SetNormal( (camT->GetPosition() - refGoCenter).NormalizedSafe() );
        }

        // Then cast a ray through the mouse position, and see where it intersects
        // with this plane.
        Ray mouseRay = cam->FromViewportPointNDCToRay( Input::GetMousePositionNDC() );

        bool intersected;
        Vector3 intersection;
        Geometry::RayPlane(mouseRay, plane, &intersected, &intersection);

        if (GetAxis() != Axis3DExt::XYZ)
        {
            if (intersected)
            {
                Vector3 centerToMousePointV = (intersection - refGoCenter);
                Vector3 centerToMousePointProjV =
                    centerToMousePointV.ProjectedOnVector(GetAxisVectorWorld());

                Vector3 centerToMousePointProjLocalV =
                        refGoT->FromWorldToLocalDirection(centerToMousePointProjV);

                if (GrabHasJustChanged())
                {
                    m_startGrabCenterToMousePointProjLocalV = centerToMousePointProjLocalV;
                }

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
        else // XYZ
        {
            Vector3 diff = (intersection - refGoCenter);
            float scaling = (diff.x + diff.y);
            scaling += 1.0f;

            refGoT->SetLocalScale(m_startGrabLocalScale * scaling);
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

void ScaleGizmoAxis::SetAxis(Axis3DExt axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdatePoints(1.0f);
}

void ScaleGizmoAxis::UpdatePoints(float localAxisLength)
{
    if (!GetParent()) { return; }

    Vector3 axisFwd = GetAxisVectorLocal() * localAxisLength;
    if (GetAxis() == Axis3DExt::XYZ) { axisFwd = Vector3::Zero; }

    p_lineRenderer->SetPoints( {Vector3::Zero, axisFwd} );
    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);

    float baseScale = 0.2f;
    if (GetAxis() == Axis3DExt::XYZ) { baseScale += 0.4f; }
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
