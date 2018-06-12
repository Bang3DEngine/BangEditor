#include "BangEditor/TransformGizmoAxis.h"

#include "Bang/Ray.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/AxisFunctions.h"

#include "BangEditor/EditorCamera.h"
#include "BangEditor/TransformGizmoAxis.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TransformGizmoAxis::TransformGizmoAxis()
{
    SetName("TransformGizmoAxis");

    AddComponent<Transform>();
}

TransformGizmoAxis::~TransformGizmoAxis()
{
}

void TransformGizmoAxis::Update()
{
    SelectionGizmo::Update();

    // Change color depending on selection state
    SetColor( GetSelectionState() );

    // Block editor camera if being grabbed
    if (EditorCamera *edCam = EditorCamera::GetInstance())
    {
        if (IsBeingGrabbed())
        {
            edCam->RequestBlockBy(this);
        }
        else
        {
            edCam->RequestUnBlockBy(this);
        }
    }
}

void TransformGizmoAxis::SetAxis(Axis3DExt axis)
{
    if (axis != GetAxis())
    {
        m_axis = axis;
        SetColor(SelectionState::IDLE);
    }
}
Axis3DExt TransformGizmoAxis::GetAxis() const { return m_axis; }
Vector3 TransformGizmoAxis::GetAxisVectorLocal() const
{
    return GetAxisVector( GetAxis() );
}

Vector3 TransformGizmoAxis::GetAxisVectorWorld() const
{
    Transform *refGoT = GetReferencedGameObject()->GetTransform();
    return refGoT->FromLocalToWorldDirection( GetAxisVectorLocal() ).Normalized();
}

void TransformGizmoAxis::SetColor(SelectionState state)
{
    constexpr float DotRange = 0.2f;

    float alpha = 1.0f;
    if (ApplyAlignmentAlpha())
    {
        float dot = 1.0f;
        GameObject *refGo = GetReferencedGameObject();
        EditorCamera *edCam = EditorCamera::GetInstance();
        if (edCam && refGo)
        {
            Vector3 refGoPos = refGo->GetTransform()->GetPosition();
            Vector3 edCamPos = edCam->GetTransform()->GetPosition();
            Vector3 edCamToRefGoDir = (refGoPos - edCamPos).NormalizedSafe();
            if (edCamToRefGoDir.Length() > 0)
            {
                dot = Math::Abs( Vector3::Dot(GetAxisVectorWorld(), edCamToRefGoDir) );
            }
        }

        if ((1.0f-dot) <= DotRange)
        {
            alpha = ((1.0f-dot) / DotRange);
        }
    }
    SetVisible( (alpha >= 0.1f) );

    switch (state)
    {
        case SelectionState::IDLE:
            SetColor( GetAxisColor( GetAxis() ).WithAlpha(alpha) );
            break;

        case SelectionState::OVER:
            SetColor( Color::Orange );
            break;

        case SelectionState::GRABBED:
            SetColor( Color::Yellow );
            break;
    }
}

void TransformGizmoAxis::OnDisabled()
{
    SelectionGizmo::OnDisabled();
    SetColor(SelectionState::IDLE);
}

bool TransformGizmoAxis::ApplyAlignmentAlpha() const
{
    return true;
}
