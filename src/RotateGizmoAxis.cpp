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
#include "Bang/DebugRenderer.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/TransformGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RotateGizmoAxis::RotateGizmoAxis()
{
    SetName("RotateGizmoAxis");

    p_circleRenderer = AddComponent<LineRenderer>();
    p_circleRenderer->SetRenderPass(RenderPass::Gizmos);
    p_circleRenderer->SetLineWidth(2.0f);

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetRenderPass(RenderPass::Gizmos);
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

void RotateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if ( IsBeingGrabbed())
    {
        GameObject *refGo = GetReferencedGameObject();
        Transform *refGoT = refGo->GetTransform();
        Vector3 sphereCenter = refGoT->GetPosition();

        Camera *cam = Camera::GetActive();
        Ray mouseRay = cam->FromViewportPointNDCToRay( Input::GetMousePositionNDC() );

        bool intersected;
        Vector3 spherePoint;
        TransformGizmo *tg = SCAST<TransformGizmo*>(GetParent()->GetParent());
        const float SphereRadius = tg->GetScaleFactor() * 1.0f;
        mouseRay.GetIntersectionWithSphere(sphereCenter, SphereRadius,
                                           &intersected, &spherePoint);
        if (!intersected)
        {
            Vector3 closestRayPointToSphere = mouseRay.GetClosestPointToPoint(sphereCenter);
            spherePoint = GetProjectedPointIntoSphere(closestRayPointToSphere,
                                                      sphereCenter, SphereRadius);
        }

        if (GrabHasJustChanged())
        {
            // Get the sphere point but constrained on the axis (over rotation plane)
            Vector3 axisedSpherePoint = GetAxisedSpherePoint(spherePoint,
                                                             GetAxisVectorWorld(),
                                                             sphereCenter,
                                                             SphereRadius);

            // Obtain the sphere tangent vector T on spherePoint coplanar to P,
            // which will determine the direction that will drive user movement...
            // If he moves mouse in direction T,  rotation speed will be max.
            // If he moves it perpendicular to T, rotation speed will be 0.
            Vector3 rotationTangentDirection =
                    Vector3::Cross(sphereCenter - axisedSpherePoint,
                                   GetAxisVectorWorld());
            rotationTangentDirection.Normalize();

            m_startingGrabMousePosNDC = Input::GetMousePositionNDC();
            m_startingGrabSphereTangentDir = rotationTangentDirection;
            m_startingGrabAxisedSpherePoint = axisedSpherePoint;
        }


        // Get mouseAxis, and displace from the starting grab mouse pos. With
        // this displaced mouse position, get the new point in the sphere
        Vector2 mouseAxis = Input::GetMouseAxis();
        Vector2 displacedMousePosition = m_startingGrabMousePosNDC + mouseAxis;
        Ray displacedMouseRay = cam->FromViewportPointNDCToRay(displacedMousePosition);
        Vector3 displacedMouseRayClosestPointInTangentLine;
        displacedMouseRay.GetClosestPointToLine(
                    m_startingGrabAxisedSpherePoint,
                    m_startingGrabSphereTangentDir,
                    nullptr, &displacedMouseRayClosestPointInTangentLine);

        const Vector3 &dmrcpitl = displacedMouseRayClosestPointInTangentLine;
        Vector3 displacedAxisedSpherePoint =
           GetProjectedPointIntoSphere(dmrcpitl, sphereCenter, SphereRadius);

        // Now that we have the two points (starting and new displaced), get
        // the 2 vectors from the center outwards, and just apply the rotation
        // using a Quaternion From starting To new_displaced_sphere_position
        Vector3 sphereCenterToStartAxisedSpherePoint =
                (m_startingGrabAxisedSpherePoint - sphereCenter);
        Vector3 sphereCenterToCurrentAxisedSpherePoint =
                (displacedAxisedSpherePoint - sphereCenter);

        // Finally, apply the rotation
        Quaternion rotQ = Quaternion::FromTo(sphereCenterToStartAxisedSpherePoint,
                                             sphereCenterToCurrentAxisedSpherePoint);
        if ( rotQ.GetEulerAngles().ToDegrees().Length() > 2) // Avoid precision issues
        {
            refGoT->Rotate(rotQ);
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

void RotateGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
    UpdateCirclePoints();
}

void RotateGizmoAxis::UpdateCirclePoints()
{
    if (!GetReferencedGameObject()) { return; }

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
