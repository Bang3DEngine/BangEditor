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
}

void TransformGizmoAxis::SetAxis(Axis3DExt axis)
{
    if (axis != GetAxis())
    {
        m_axis = axis;
        SetColor(SelectionState::Idle);
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
    switch (state)
    {
        case SelectionState::Idle:
            SetColor( GetAxisColor( GetAxis() ) );
            break;

        case SelectionState::Over:
            SetColor(Color::Orange);
            break;

        case SelectionState::Grabbed:
            SetColor(Color::Yellow);
            break;
    }
}

void TransformGizmoAxis::OnDisabled()
{
    SelectionGizmo::OnDisabled();
    SetColor(SelectionState::Idle);
}
