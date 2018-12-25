#include "BangEditor/ScaleGizmoAxis.h"

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/AxisFunctions.h"
#include "Bang/Camera.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Geometry.h"
#include "Bang/Input.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"
#include "Bang/Mesh.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Plane.h"
#include "Bang/Ray.h"
#include "Bang/Transform.h"
#include "Bang/Vector3.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/Selection.h"

namespace Bang
{
class Color;
}

using namespace Bang;
using namespace BangEditor;

float ScaleGizmoAxis::ArrowCapScale = 0.3f;

ScaleGizmoAxis::ScaleGizmoAxis()
{
    SetName("ScaleGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    p_meshRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_meshRenderer->GetMaterial()->SetReceivesLighting(false);
    p_meshRenderer->SetMesh(MeshFactory::GetCube().Get());
    p_meshRenderer->SetCastsShadows(false);
    p_meshRenderer->SetReceivesShadows(false);
    p_arrowCap->GetTransform()->SetLocalScale(Vector3(ArrowCapScale));

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");
    p_selectionGo->SetVisible(false);

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_selectionRenderer->SetMesh(MeshFactory::GetCube().Get());
    p_selectionRenderer->SetCastsShadows(false);
    p_selectionRenderer->SetReceivesShadows(false);

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

#include "Bang/Debug.h"
void ScaleGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    GameObject *refGo = GetReferencedGameObject();
    if (!refGo || !refGo->GetTransform())
    {
        return;
    }

    const Vector3 axis = GetAxisVectorWorld();
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
            plane.SetNormal(
                Vector3::Cross(axis, Vector3::Cross(camT->GetForward(), axis))
                    .Normalized());
        }
        else
        {
            // Plane parallel to camera plane
            plane.SetNormal(
                (camT->GetPosition() - refGoCenter).NormalizedSafe());
        }

        // Then cast a ray through the mouse position, and see where it
        // intersects
        // with this plane.
        Ray mouseRay =
            cam->FromViewportPointNDCToRay(Input::GetMousePositionNDC());

        bool intersected;
        Vector3 intersection;
        Geometry::IntersectRayPlane(
            mouseRay, plane, &intersected, &intersection);

        if (GetAxis() != Axis3DExt::XYZ)
        {
            if (intersected)
            {
                Vector3 centerToMousePointProjV =
                    (intersection - refGoCenter).ProjectedOnVector(axis);

                if (GrabHasJustChanged())
                {
                    m_startArrowCapProjPos = (p_arrowCap->GetTransform()
                                                  ->GetPosition()
                                                  .ProjectedOnVector(axis));
                    m_startGrabOffsetFromArrowCapPos =
                        (m_startArrowCapProjPos -
                         refGoCenter.ProjectedOnVector(axis)) -
                        centerToMousePointProjV;
                }

                centerToMousePointProjV += m_startGrabOffsetFromArrowCapPos;

                Vector3 centerToMousePointProjLocalV =
                    refGoT->FromWorldToLocalDirection(centerToMousePointProjV);

                float startAxisProjLength =
                    (m_startArrowCapProjPos -
                     refGoCenter.ProjectedOnVector(axis))
                        .Length();

                Vector3 normCenterToMousePointProjLocalV =
                    (centerToMousePointProjLocalV / startAxisProjLength);
                normCenterToMousePointProjLocalV.z *= -1.0f;
                Vector3 scaleDelta = Vector3::Abs(GetAxisVectorLocal()) *
                                     normCenterToMousePointProjLocalV;
                scaleDelta +=
                    Vector3(1) - GetAxisVectorLocal() * Vector3(1, 1, -1);

                Vector3 newScale = m_startGrabLocalScale * scaleDelta;
                refGoT->SetLocalScale(newScale);

                // Update gizmo length
                const int i = GetAxisIndex(GetAxis());
                Vector3 centerToMousePointProjLocalGizmoV =
                    GetTransform()->FromWorldToLocalVector(
                        centerToMousePointProjV);
                centerToMousePointProjLocalGizmoV *= Vector3(1, 1, -1);
                UpdatePoints(centerToMousePointProjLocalGizmoV[i]);
            }
        }
        else  // XYZ
        {
            Vector3 diff = (intersection - refGoCenter);

            EditorCamera *edCam = EditorCamera::GetInstance();
            Vector3 diffProj =
                edCam->GetCamera()->FromWorldPointToViewportPointNDC(diff);

            if (GrabHasJustChanged())
            {
                m_startGrabOffsetFromArrowCapPos = diffProj;
            }
            diffProj -= m_startGrabOffsetFromArrowCapPos;

            float scaling = (diffProj.x + diffProj.y);
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
    TransformGizmoAxis::Render(renderPass, renderChildren);
}

void ScaleGizmoAxis::SetAxis(Axis3DExt axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdatePoints(1.0f);
}

void ScaleGizmoAxis::UpdatePoints(float localAxisLength)
{
    if (!GetParent())
    {
        return;
    }

    Vector3 axisFwd = GetAxisVectorLocal() * localAxisLength;
    if (GetAxis() == Axis3DExt::XYZ)
    {
        axisFwd = Vector3::Zero();
    }

    p_lineRenderer->SetPoints({Vector3::Zero(), axisFwd});
    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);

    float baseScale = 0.35f;
    if (GetAxis() == Axis3DExt::XYZ)
    {
        baseScale += 0.2f;
    }
    p_selectionGo->GetTransform()->SetLocalScale(Vector3(baseScale) +
                                                 Vector3::Abs(axisFwd));
    p_selectionGo->GetTransform()->SetLocalPosition(axisFwd *
                                                    (1.0f + baseScale) * 0.5f);
}

GameObject *ScaleGizmoAxis::GetSelectionGameObject() const
{
    return p_selectionGo;
}

bool ScaleGizmoAxis::ApplyAlignmentAlpha() const
{
    return (GetAxis() != Axis3DExt::XYZ);
}

void ScaleGizmoAxis::SetColor(const Color &color)
{
    p_lineRenderer->GetMaterial()->SetAlbedoColor(color);
    p_meshRenderer->GetMaterial()->SetAlbedoColor(color);
}
