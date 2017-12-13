#include "BangEditor/RotateGizmoAxis.h"

#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/LineRenderer.h"
#include "Bang/DebugRenderer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RotateGizmoAxis::RotateGizmoAxis()
{
    SetName("RotateGizmoAxis");

    p_circleRenderer = AddComponent<LineRenderer>();
    p_circleRenderer->SetRenderPass(RenderPass::Gizmos);
    p_circleRenderer->SetLineWidth(2.0f);
}

RotateGizmoAxis::~RotateGizmoAxis()
{
}

void RotateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();
    UpdateCirclePoints();
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
            case Axis3D::X: newPoint.x = sin; newPoint.y = cos; break;
            case Axis3D::Y: newPoint.x = sin; newPoint.z = cos; break;
            case Axis3D::Z: newPoint.y = sin; newPoint.z = cos; break;
        }

        // Cull non-facing to camera
        Vector3 refGoCenter = GetReferencedGameObject()->GetTransform()->GetPosition();
        Vector3 newPointWorld = GetTransform()->FromLocalToWorldPoint(newPoint);
        Vector3 refGoCenterToLine = (newPointWorld - refGoCenter).Normalized();
        Vector3 camToLine = (newPointWorld - camT->GetPosition()).Normalized();
        float dot = Vector3::Dot(camToLine, refGoCenterToLine);
        if (dot <= 0.2f) { circlePoints.PushBack(newPoint); }
    }

    Array<Vector3> rendererPoints;
    if (!circlePoints.IsEmpty())
    {
        // Fill renderer points
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
}

void RotateGizmoAxis::SetColor(const Color &color)
{
    p_circleRenderer->GetMaterial()->SetDiffuseColor(color);
}
