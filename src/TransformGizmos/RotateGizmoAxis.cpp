#include "BangEditor/RotateGizmoAxis.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Resources.h"
#include "Bang/Transform.h"
#include "Bang/MeshFactory.h"
#include "Bang/LineRenderer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/AxisFunctions.h"
#include "Bang/DebugRenderer.h"
#include "Bang/RendererFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/TransformGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RotateGizmoAxis::RotateGizmoAxis()
{
    SetName("RotateGizmoAxis");

    p_circleRenderer = AddComponent<LineRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_circleRenderer);
    p_circleRenderer->SetLineWidth(2.0f);

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_selectionRenderer);
    p_selectionRenderer->SetCulling(false);

    m_selectionMesh = Resources::Create<Mesh>();

    p_selectionGo->SetParent(this);
}

RotateGizmoAxis::~RotateGizmoAxis()
{
}

Vector3 GetProjectedPointIntoSphere(const Vector3 &point,
                                    const Vector3 &sphereCenter,
                                    float sphereRadius)
{
    Vector3 closestRayPointToSphereV = sphereCenter - point;
    Vector3 closestRayPointToSphereDir = closestRayPointToSphereV.Normalized();
    return sphereCenter - closestRayPointToSphereDir * sphereRadius;
}

Vector3 GetAxisedSpherePoint(const Vector3 &spherePoint,
                             const Vector3 &axisWorld,
                             const Vector3 &sphereCenter,
                             float sphereRadius)
{
    // Returns the point in the sphere over the specified axis (i.e., on the
    // plane with normal axisWorld)
    Vector3 axisedSpherePoint = spherePoint.ProjectedOnPlane(axisWorld,
                                                             sphereCenter);
    return sphereCenter + (axisedSpherePoint - sphereCenter).Normalized() *
           sphereRadius;
}

Vector3 GetAxisedSpherePointFromMousePosNDC(const Camera *cam,
                                            const Vector2 &mousePosNDC,
                                            const Vector3 &axis,
                                            const Vector3 &sphereCenter,
                                            float sphereRadius)
{
    Ray mouseRay = cam->FromViewportPointNDCToRay(mousePosNDC);

    // Throw a ray through mouse position into scene, to see if it intersects
    // with the sphere
    bool intersected;
    Vector3 spherePoint;
    mouseRay.GetIntersectionWithSphere(sphereCenter, sphereRadius,
                                       &intersected, &spherePoint);
    if (!intersected)
    {
        // If it did not intersect, find closest sphere point to ray
        Vector3 closestRayPointToSphere = mouseRay.GetClosestPointToPoint(sphereCenter);
        spherePoint = GetProjectedPointIntoSphere(closestRayPointToSphere,
                                                  sphereCenter, sphereRadius);
    }

    // Get the sphere point but constrained on the axis (over rotation plane)
    Vector3 axisedSpherePoint = GetAxisedSpherePoint(spherePoint, axis,
                                                     sphereCenter,
                                                     sphereRadius);

    return axisedSpherePoint;
}

void RotateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (!GetReferencedGameObject()->GetTransform()) { return; }

    if ( IsBeingGrabbed() )
    {
        GameObject *refGo = GetReferencedGameObject();
        Transform *refGoT = refGo->GetTransform();
        Vector3 sphereCenter = refGoT->GetPosition();

        Camera *cam = Camera::GetActive();
        const Vector2 mousePos = Input::GetMousePositionNDC();

        TransformGizmo *tg = SCAST<TransformGizmo*>(GetParent()->GetParent());
        const float SphereRadius = tg->GetScaleFactor() * 1.0f;

        if (GrabHasJustChanged())
        {
            m_startingGrabMousePosNDC = Input::GetMousePositionNDC();
            m_startingGrabAxisedSpherePoint =
                    GetAxisedSpherePointFromMousePosNDC(cam, mousePos,
                                                        GetAxisVectorWorld(),
                                                        sphereCenter,
                                                        SphereRadius);
        }
        else
        {
            // Get mouseAxis, and displace from the starting grab mouse pos. With
            // this displaced mouse position, get the new point in the sphere
            Vector2 mouseAxis = Input::GetMouseAxis();
            Vector2 displacedMousePosition = m_startingGrabMousePosNDC + mouseAxis;
            Vector3 displacedMouseAxisedSpherePoint =
                    GetAxisedSpherePointFromMousePosNDC(cam,
                                                        displacedMousePosition,
                                                        GetAxisVectorWorld(),
                                                        sphereCenter,
                                                        SphereRadius);

            // Now that we have the two points (starting and new displaced), get
            // the 2 vectors from the center outwards, and just apply the rotation
            // using a Quaternion From starting To new_displaced_sphere_position
            Vector3 sphereCenterToStartAxisedSpherePoint =
                    (m_startingGrabAxisedSpherePoint - sphereCenter);
            Vector3 sphereCenterToCurrentAxisedSpherePoint =
                    (displacedMouseAxisedSpherePoint - sphereCenter);

            Vector3 startDirLocal = GetTransform()->FromWorldToLocalDirection(
                                            sphereCenterToStartAxisedSpherePoint);
            Vector3 newDirLocal = GetTransform()->FromWorldToLocalDirection(
                                            sphereCenterToCurrentAxisedSpherePoint);
            startDirLocal.Normalize();
            newDirLocal.Normalize();

            // Only process now if dirs are different enough (avoid NaN's)
            if ( Math::Abs( Vector3::Dot(startDirLocal, newDirLocal) ) < 0.9999f)
            {
                // Get the quaternion representing the local rotation displacement
                Quaternion deltaLocalRot = Quaternion::FromTo(startDirLocal,
                                                              newDirLocal);

                // Apply Snapping if demanded with shift
                if (Input::GetKey(Key::LShift))
                {
                    constexpr float SnappingDeg = 15.0f;
                    Vector3 deltaEulerDeg = deltaLocalRot.GetEulerAngles()
                                            .ToDegrees();

                    const int i = GetAxisIndex(GetAxis());
                    float snappingDeg = Math::Round(deltaEulerDeg[i] /
                                                    SnappingDeg) * SnappingDeg;
                    float snappingRad = Math::DegToRad(snappingDeg);

                    deltaLocalRot = Quaternion::AngleAxis(snappingRad,
                                            Vector3::Abs(GetAxisVectorLocal()));
                }

                // Finally, apply the rotation
                deltaLocalRot = GetQuaternionAxised(deltaLocalRot, GetAxis());
                refGoT->RotateLocal(deltaLocalRot);
            }
        }
    }
}

void RotateGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionGo->SetEnabled(selection);

    UpdateCirclePoints();

    TransformGizmoAxis::Render(renderPass, renderChildren);
}

Quaternion RotateGizmoAxis::GetQuaternionAxised(const Quaternion &q, Axis3D axis)
{
    Vector3 qAngleAxis = q.GetAngleAxis();
    auto other = GetOtherAxisIndex( axis );
    qAngleAxis[other.first] = qAngleAxis[other.second] = 0.0f;
    return Quaternion::AngleAxis(qAngleAxis.Length(),
                                 qAngleAxis.NormalizedSafe());
}

void RotateGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdateCirclePoints();
}

void RotateGizmoAxis::UpdateCirclePoints()
{
    if (!GetReferencedGameObject() ||
        !GetReferencedGameObject()->GetTransform()) { return; }

    Camera *cam = Camera::GetActive();
    Transform *camT = cam->GetGameObject()->GetTransform();

    constexpr int numSegments = 32;
    constexpr float angleStep = (Math::Pi * 2.0f) / numSegments;

    // Get circle points
    Array<Vector3> circlePoints;
    for (uint i = 0; i < numSegments + 1; ++i)
    {
        float angle = angleStep * i;
        Vector3 newPoint = Vector3::Zero;
        float sin = Math::Sin(angle);
        float cos = Math::Cos(angle);
        switch ( GetAxis() )
        {
            case Axis3D::X: newPoint.y = sin; newPoint.z = cos; break;
            case Axis3D::Y: newPoint.x = sin; newPoint.z = cos; break;
            case Axis3D::Z: newPoint.x = sin; newPoint.y = cos; break;
        }

        // Cull non-facing to camera
        Vector3 refGoCenter = GetReferencedGameObject()->GetTransform()->GetPosition();
        Vector3 newPointWorld = GetTransform()->FromLocalToWorldPoint(newPoint);
        Vector3 refGoCenterToLine = (newPointWorld - refGoCenter).Normalized();
        Vector3 camToLine = (newPointWorld - camT->GetPosition()).Normalized();
        float dot = Vector3::Dot(camToLine, refGoCenterToLine);
        if (dot <= 0.2f) { circlePoints.PushBack(newPoint); }
    }

    // Fill renderer points
    Array<Vector3> rendererPoints;
    if (!circlePoints.IsEmpty())
    {
        for (uint i = 0; i < circlePoints.Size() - 1; ++i)
        {
            const Vector3 &p0 = circlePoints[i+0];
            const Vector3 &p1 = circlePoints[i+1];
            if ( Vector3::Distance(p0, p1) < 0.5f ) // Avoid linking big jumps
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
            const Vector3 &p0 = rendererPoints[i+0];
            const Vector3 &p1 = rendererPoints[i+1];

            Vector3 p0p1  = (p1-p0);
            Vector3 norm0_c = (p0 - Vector3::Zero).Normalized() *
                               SelectionMeshThickness;
            Vector3 norm1_c = (p1 - Vector3::Zero).Normalized() *
                               SelectionMeshThickness;
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
        sMesh->LoadPositions(selectionMeshPoints);
        p_selectionRenderer->SetMesh(sMesh);
    }
}

void RotateGizmoAxis::SetColor(const Color &color)
{
    p_circleRenderer->GetMaterial()->SetDiffuseColor(color);
}
