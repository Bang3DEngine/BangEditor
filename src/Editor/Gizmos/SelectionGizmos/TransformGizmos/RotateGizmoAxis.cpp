#include "BangEditor/RotateGizmoAxis.h"

#include <sys/types.h>
#include <utility>

#include "Bang/Array.h"
#include "BangMath/Plane.h"
#include "Bang/Array.tcc"
#include "Bang/Assert.h"
#include "Bang/Assets.h"
#include "Bang/Assets.tcc"
#include "Bang/AxisFunctions.h"
#include "Bang/Camera.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "BangMath/Geometry.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"
#include "BangMath/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "BangMath/Ray.h"
#include "Bang/ShaderProgram.h"
#include "BangMath/Sphere.h"
#include "Bang/Transform.h"
#include "BangMath/Vector2.h"
#include "BangMath/Vector3.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/Selection.h"
#include "BangEditor/TransformGizmo.h"

namespace Bang
{
template <typename>
class ColorG;
}

using namespace Bang;
using namespace BangEditor;

RotateGizmoAxis::RotateGizmoAxis()
{
    SetName("RotateGizmoAxis");

    p_circleRenderer = AddComponent<LineRenderer>();
    p_circleRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_circleRenderer->GetMaterial()->GetShaderProgramProperties().SetLineWidth(
        2.0f);
    p_circleRenderer->GetGameObject()->AddComponent<NotSelectableInEditor>();

    m_selectionMesh = Assets::Create<Mesh>();

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("RotateGizmoAxisSelection");
    p_selectionGo->SetVisible(false);
    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_selectionRenderer->GetMaterial()
        ->GetShaderProgramProperties()
        .SetCullFace(GL::CullFaceExt::NONE);
    p_selectionRenderer->SetMesh(m_selectionMesh.Get());

    p_selectionGo->SetParent(this);
}

RotateGizmoAxis::~RotateGizmoAxis()
{
}

Vector3 GetAxisedSpherePoint(const Vector3 &spherePoint,
                             const Vector3 &axisWorld,
                             const Sphere &sphere)
{
    // Returns the point in the sphere over the specified axis (i.e., on the
    // plane with normal axisWorld)
    Vector3 axisedSpherePoint =
        spherePoint.ProjectedOnPlane(axisWorld, sphere.GetCenter());
    return sphere.GetCenter() +
           (axisedSpherePoint - sphere.GetCenter()).NormalizedSafe() *
               sphere.GetRadius();
}

Vector3 GetAxisedSpherePointFromMousePosNDC(const Camera *cam,
                                            const Vector2 &mousePosNDC,
                                            const Vector3 &axis,
                                            const Sphere &sphere)
{
    Ray mouseRay = cam->FromViewportPointNDCToRay(mousePosNDC);

    // Throw a ray through mouse position into scene, to see if it intersects
    // with the sphere
    bool intersected;
    Vector3 spherePoint;
    Geometry::IntersectRaySphere(mouseRay, sphere, &intersected, &spherePoint);
    if (!intersected)
    {
        // If it did not intersect, find closest sphere point to ray
        Vector3 closestRayPointToSphere =
            Geometry::RayClosestPointTo(mouseRay, sphere.GetCenter());
        spherePoint =
            Geometry::PointProjectedToSphere(closestRayPointToSphere, sphere);
    }

    // Get the sphere point but constrained on the axis (over rotation plane)
    Vector3 axisedSpherePoint = GetAxisedSpherePoint(spherePoint, axis, sphere);

    return axisedSpherePoint;
}

void RotateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (!GetReferencedGameObject() ||
        !GetReferencedGameObject()->GetTransform())
    {
        return;
    }

    if (IsBeingGrabbed())
    {
        GameObject *refGo = GetReferencedGameObject();
        Transform *refGoT = refGo->GetTransform();

        Camera *cam = GetEditorCamera();
        const Vector2 mousePos = Input::GetMousePositionNDC();

        TransformGizmo *tg =
            SCAST<TransformGizmo *>(GetParent()->GetParent()->GetParent());

        Sphere rotationSphere;
        rotationSphere.SetCenter(refGoT->GetPosition());
        rotationSphere.SetRadius(tg->GetScaleFactor() * 1.0f);

        const Vector3 axis =
            IsLocal() ? GetAxisVectorWorld() : GetAxisVectorLocal();

        if (GrabHasJustChanged())
        {
            m_startingGrabMousePosNDC = Input::GetMousePositionNDC();
            m_startingGrabAxisedSpherePoint =
                GetAxisedSpherePointFromMousePosNDC(
                    cam, mousePos, axis, rotationSphere);
        }
        else
        {
            // Get mouseAxis, and displace from the starting grab mouse pos.
            // With
            // this displaced mouse position, get the new point in the sphere
            Vector2 mouseAxis = Input::GetMouseAxis();
            Vector2 displacedMousePositionNDC =
                m_startingGrabMousePosNDC + mouseAxis;
            Vector3 displacedMouseAxisedSpherePoint =
                GetAxisedSpherePointFromMousePosNDC(
                    cam, displacedMousePositionNDC, axis, rotationSphere);

            // Now that we have the two points (starting and new displaced), get
            // the 2 vectors from the center outwards, and just apply the
            // rotation
            // using a Quaternion From starting To new_displaced_sphere_position
            Vector3 sphereCenterToStartAxisedSpherePoint =
                (m_startingGrabAxisedSpherePoint - rotationSphere.GetCenter());
            Vector3 sphereCenterToCurrentAxisedSpherePoint =
                (displacedMouseAxisedSpherePoint - rotationSphere.GetCenter());

            Vector3 startDirLocal = GetTransform()->FromWorldToLocalDirection(
                sphereCenterToStartAxisedSpherePoint);
            Vector3 newDirLocal = GetTransform()->FromWorldToLocalDirection(
                sphereCenterToCurrentAxisedSpherePoint);
            startDirLocal.Normalize();
            newDirLocal.Normalize();

            // Only process now if dirs are different enough (avoid NaN's)
            if (Math::Abs(Vector3::Dot(startDirLocal, newDirLocal)) < 0.9999f)
            {
                // Get the quaternion representing the local rotation
                // displacement
                Quaternion deltaLocalRot =
                    Quaternion::FromTo(startDirLocal, newDirLocal);

                // Apply Snapping if demanded with shift
                if (Input::GetKey(Key::LSHIFT))
                {
                    constexpr float SnappingDeg = 15.0f;
                    Vector3 deltaEulerDeg =
                        deltaLocalRot.GetEulerAnglesDegrees().ToDegrees();

                    const int i = GetAxisIndex(GetAxis());
                    float snappingDeg =
                        Math::Round(deltaEulerDeg[i] / SnappingDeg) *
                        SnappingDeg;
                    float snappingRad = Math::DegToRad(snappingDeg);

                    deltaLocalRot = Quaternion::AngleAxis(
                        snappingRad, Vector3::Abs(GetAxisVectorLocal()));
                }

                // Finally, apply the rotation
                deltaLocalRot = GetQuaternionAxised(deltaLocalRot, GetAxis());
                if (IsLocal())
                {
                    refGoT->RotateLocal(deltaLocalRot);
                }
                else
                {
                    refGoT->Rotate(deltaLocalRot);
                }
            }
        }
    }
}

void RotateGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    UpdateCirclePoints();
    TransformGizmoAxis::Render(renderPass, renderChildren);
}

Quaternion RotateGizmoAxis::GetQuaternionAxised(const Quaternion &q,
                                                Axis3DExt axis)
{
    ASSERT(axis == Axis3DExt::X || axis == Axis3DExt::Y ||
           axis == Axis3DExt::Z);

    Vector3 qAngleAxis = q.GetAngleAxis();
    auto other = GetOtherAxisIndex(SCAST<Axis3D>(axis));
    qAngleAxis[other.first] = qAngleAxis[other.second] = 0.0f;
    return Quaternion::AngleAxis(qAngleAxis.Length(),
                                 qAngleAxis.NormalizedSafe());
}

GameObject *RotateGizmoAxis::GetSelectionGameObject() const
{
    return p_selectionGo;
}

bool RotateGizmoAxis::ApplyAlignmentAlpha() const
{
    return false;
}

void RotateGizmoAxis::SetAxis(Axis3DExt axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdateCirclePoints();
}

void RotateGizmoAxis::UpdateCirclePoints()
{
    if (!GetReferencedGameObject() ||
        !GetReferencedGameObject()->GetTransform())
    {
        return;
    }

    Camera *cam = GetEditorCamera();
    Transform *camT = cam->GetGameObject()->GetTransform();

    constexpr int numSegments = 32;
    constexpr float angleStep = ((Math::Pi<float>() * 2.0f) / numSegments);

    // Get circle points
    Array<Vector3> circlePoints;
    for (uint i = 0; i < numSegments + 1; ++i)
    {
        float angle = angleStep * i;
        Vector3 newPoint = Vector3::Zero();
        float sin = Math::Sin(angle);
        float cos = Math::Cos(angle);
        switch (GetAxis())
        {
            case Axis3DExt::X:
                newPoint.y = sin;
                newPoint.z = cos;
                break;
            case Axis3DExt::Y:
                newPoint.x = sin;
                newPoint.z = cos;
                break;
            case Axis3DExt::Z:
                newPoint.x = sin;
                newPoint.y = cos;
                break;
            default: ASSERT(false);
        }

        // Cull non-facing to camera
        Vector3 refGoCenter =
            GetReferencedGameObject()->GetTransform()->GetPosition();
        Vector3 newPointWorld = GetTransform()->FromLocalToWorldPoint(newPoint);
        Vector3 refGoCenterToLine =
            (newPointWorld - refGoCenter).NormalizedSafe();
        Vector3 camToLine =
            (newPointWorld - camT->GetPosition()).NormalizedSafe();
        float dot = Vector3::Dot(camToLine, refGoCenterToLine);
        if (dot <= 0.2f)
        {
            circlePoints.PushBack(newPoint);
        }
    }

    // Fill renderer points
    Array<Vector3> rendererPoints;
    if (!circlePoints.IsEmpty())
    {
        for (uint i = 0; i < circlePoints.Size() - 1; ++i)
        {
            const Vector3 &p0 = circlePoints[i + 0];
            const Vector3 &p1 = circlePoints[i + 1];
            if (Vector3::Distance(p0, p1) < 0.5f)  // Avoid linking big jumps
            {
                rendererPoints.PushBack(p0);
                rendererPoints.PushBack(p1);
            }
        }
    }
    p_circleRenderer->SetPoints(rendererPoints);

    // Create selection mesh (box wrapping around the culled circle)
    if (!rendererPoints.IsEmpty())
    {
        constexpr float SelectionMeshThickness = 0.15f;
        Array<Vector3> selectionMeshPoints;
        Mesh *sMesh = m_selectionMesh.Get();
        for (uint i = 0; i < rendererPoints.Size() - 1; i += 2)
        {
            const Vector3 &p0 = rendererPoints[i + 0];
            const Vector3 &p1 = rendererPoints[i + 1];

            Vector3 p0p1 = (p1 - p0);
            Vector3 norm0_c =
                (p0 - Vector3::Zero()).Normalized() * SelectionMeshThickness;
            Vector3 norm1_c =
                (p1 - Vector3::Zero()).Normalized() * SelectionMeshThickness;
            Vector3 norm0_n = Vector3::Cross(p0p1, norm0_c).Normalized() *
                              SelectionMeshThickness;
            Vector3 norm1_n = Vector3::Cross(p0p1, norm1_c).Normalized() *
                              SelectionMeshThickness;

            selectionMeshPoints.PushBack(p0 - norm0_c + norm0_n);
            selectionMeshPoints.PushBack(p0 + norm0_c + norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c + norm1_n);
            selectionMeshPoints.PushBack(p0 - norm0_c + norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c + norm1_n);
            selectionMeshPoints.PushBack(p1 - norm1_c + norm1_n);

            selectionMeshPoints.PushBack(p0 - norm0_c - norm0_n);
            selectionMeshPoints.PushBack(p0 + norm0_c - norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c - norm1_n);
            selectionMeshPoints.PushBack(p0 - norm0_c - norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c - norm1_n);
            selectionMeshPoints.PushBack(p1 - norm1_c - norm1_n);

            selectionMeshPoints.PushBack(p0 + norm0_c - norm0_n);
            selectionMeshPoints.PushBack(p0 + norm0_c + norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c + norm1_n);
            selectionMeshPoints.PushBack(p0 + norm0_c - norm0_n);
            selectionMeshPoints.PushBack(p1 + norm1_c + norm1_n);
            selectionMeshPoints.PushBack(p1 + norm1_c - norm1_n);
        }
        sMesh->SetPositionsPool(selectionMeshPoints);
        m_selectionMesh.Get()->UpdateVAOs();
    }
}

void RotateGizmoAxis::SetColor(const Color &color)
{
    p_circleRenderer->GetMaterial()->SetAlbedoColor(color);
}
