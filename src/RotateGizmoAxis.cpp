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

void RotateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();
    UpdateCirclePoints();

    if ( IsBeingGrabbed())
    {
        GameObject *refGo = GetReferencedGameObject();
        Transform *refGoT = refGo->GetTransform();
        Vector3 sphereCenter = refGoT->GetPosition();

        Ray mouseRay = Camera::GetActive()->
                       FromViewportPointNDCToRay( Input::GetMousePositionNDC() );

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

            /*
            Ray closestRayPointToSphereRay;
            closestRayPointToSphereRay.SetOrigin( closestRayPointToSphere );
            closestRayPointToSphereRay.SetDirection( closestRayPointToSphereV );

            bool intersected2;
            closestRayPointToSphereRay.
                    GetIntersectionWithSphere(sphereCenter, SphereRadius,
                                              &intersected2, &spherePoint);
            ASSERT(intersected2);
            DebugRenderer::RenderLine(sphereCenter, -closestRayPointToSphereV,
                                       Color::Green, 0.5f, 1.0f, true);
            */
        }

        // DebugRenderer::RenderPoint(spherePoint, Color::Red, 0.5f, 10.0f, true);

        // Create the plane P normal to the axis (the rotation plane)
        Vector3 rotationPlaneNormal = GetAxisVectorWorld();
        Vector3 rotationPlanePoint  = sphereCenter;

        Vector3 axisedSpherePoint =
                spherePoint.ProjectedOnPlane(rotationPlaneNormal,
                                             rotationPlanePoint);
        axisedSpherePoint = sphereCenter +
                           (axisedSpherePoint - sphereCenter).Normalized() *
                            SphereRadius;
        if (GrabHasJustChanged())
        {
            // Obtain the sphere tangent vector T on spherePoint coplanar to P,
            // which will determine the direction that will drive user movement...
            // If he moves mouse in direction T,  rotation speed will be max.
            // If he moves it perpendicular to T, rotation speed will be 0.
            Vector3 rotationTangentDirection =
                    Vector3::Cross(sphereCenter - axisedSpherePoint,
                                   rotationPlaneNormal);
            rotationTangentDirection.Normalize();

            m_startingGrabMousePosNDC = Input::GetMousePositionNDC();
            m_startingGrabSphereTangentDir = rotationTangentDirection;
            m_startingGrabAxisedSpherePoint = axisedSpherePoint;
        }
/*
        DebugRenderer::RenderPoint(axisedSpherePoint,
                                   Color::Black, 0.5f, 3.0f, false);
        DebugRenderer::RenderLine(sphereCenter,
                                  axisedSpherePoint,
                                  Color::Green, 0.5f, 1.0f, false);
        DebugRenderer::RenderLine(axisedSpherePoint,
                                  axisedSpherePoint + rotationPlaneNormal,
                                  Color::Red, 0.5f, 1.0f, false);
        DebugRenderer::RenderLine(m_startingGrabAxisedSpherePoint - m_startingGrabSphereTangentDir * 99.9f,
                                  m_startingGrabAxisedSpherePoint + m_startingGrabSphereTangentDir * 99.9f,
                                  Color::Pink, 0.5f, 1.0f, true);
*/

        // Get the sphere tangent reference direction in screen NDC space
        Camera *cam = Camera::GetActive();
        /*
        Vector2 p1NDC = cam->FromWorldPointToViewportPointNDC(spherePoint);
        Vector2 p2NDC = cam->FromWorldPointToViewportPointNDC(spherePoint +
                                                              m_startingGrabSphereTangentDir);
        Vector2 worldZeroScreenNDC =
                cam->FromWorldPointToViewportPointNDC(Vector3::Zero);
        Vector2 rotationTangentPointScreenNDC =
                cam->FromWorldPointToViewportPointNDC(rotationTangentDirection);
        Vector2 rotationTangentDirScreenNDC =
                p2NDC - p1NDC;
                // cam->FromWorldPointToViewportPointNDC(rotationTangentDirection);
                // rotationTangentPointScreenNDC - worldZeroScreenNDC;
        // rotationTangentDirScreenNDC.Normalize();
        // Debug_Peek(rotationTangentDirScreenNDC);
        */

        // Get mouseAxis
        Vector2 mouseAxis = Input::GetMouseAxis();
        Vector2 displacedMousePosition = m_startingGrabMousePosNDC + mouseAxis;
        Debug_Peek(m_startingGrabMousePosNDC);
        Debug_Peek(displacedMousePosition);
        Ray displacedMouseRay = cam->FromViewportPointNDCToRay(displacedMousePosition);
        Vector3 displacedMouseRayClosestPointInTangentLine;
        displacedMouseRay.GetClosestPointToLine(
                    m_startingGrabAxisedSpherePoint,
                    m_startingGrabSphereTangentDir,
                    nullptr, &displacedMouseRayClosestPointInTangentLine);

        const Vector3 &dmrcpitl = displacedMouseRayClosestPointInTangentLine;
        Vector3 displacedAxisedSpherePoint =
            GetProjectedPointIntoSphere(dmrcpitl, sphereCenter, SphereRadius);

        Vector3 sphereCenterToStartAxisedSpherePoint =
                (m_startingGrabAxisedSpherePoint - sphereCenter);
        Vector3 sphereCenterToCurrentAxisedSpherePoint =
                (displacedAxisedSpherePoint - sphereCenter);

        DebugRenderer::RenderPoint(m_startingGrabAxisedSpherePoint,
                                   Color::Red, 0.5f, 4.0f, false);
        DebugRenderer::RenderPoint(displacedAxisedSpherePoint,
                                   Color::Green, 0.5f, 4.0f, false);

        float dot = Vector3::Dot(sphereCenterToStartAxisedSpherePoint.Normalized(),
                                 sphereCenterToCurrentAxisedSpherePoint.Normalized());
        float angle = Math::ACos(dot);
        float angleDegrees = Math::Rad2Deg(angle);
        if (angleDegrees > 2) // Avoid precision issues
        {
            refGoT->Rotate( Quaternion::FromTo(sphereCenterToStartAxisedSpherePoint,
                                               sphereCenterToCurrentAxisedSpherePoint) );
            Debug_Log("Angle: " << angleDegrees );
        }
        // GetAxisVectorWorld() * angle );

        // float dot = Vector2::Dot(mouseAxis.Normalized(),
        //                          rotationTangentDirScreenNDC);

        /*Debug_Peek(displacedMouseRayClosestAxisedPoint);
        DebugRenderer::RenderPoint(displacedMouseRayClosestAxisedPoint,
                                   Color::Green, 0.5f, 3.0f, true);
        */

        /*
        Debug_Peek(Input::GetMouseAxis());
        Debug_Peek(Input::GetMouseAxisX());
        Debug_Peek(Input::GetMouseAxisY());
        Debug_Peek(Input::GetMouseDelta());
        if (mouseAxis.Length() > 0)
        {
            Debug_Peek(mouseAxis);
            Debug_Log(Vector2::Dot(mouseAxis.Normalized(),
                                   rotationTangentDirScreenNDC));
        }
        Vector3 mouseAxisInRotationPlaneSpace =
                mouseAxis.x * ;
        bool intersectedWithRotPlane;
        Vector3 mouseAxisInRotationPlaneSpace;
        mouseRay.GetIntersectionWithPlane(rotationPlanePoint,
                                          rotationPlaneNormal,
                                          &intersectedWithRotPlane,
                                          &mouseAxisInRotationPlaneSpace);
        if (intersectedWithRotPlane)
        {
            Debug_Log(Vector3::Dot(mouseAxisInRotationPlaneSpace.Normalized(),
                                   rotationTangentDirection));
            // Now, we want to know how aligned was the user movement to that
            // of
        }
                                          */
    }
}

void RotateGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionGo->SetEnabled(selection);
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
