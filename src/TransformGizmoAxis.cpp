#include "BangEditor/TransformGizmoAxis.h"

#include "Bang/Ray.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Selection.h"
#include "Bang/Transform.h"
#include "Bang/AxisFunctions.h"

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
    GameObject::Update();

    // Change color depending on selection state
    bool isMouseOver = false;
    SelectionState selectionState = SelectionState::Idle;
    GameObject *overedGameObject = Selection::GetOveredGameObject();
    if (IsBeingGrabbed()) { selectionState = SelectionState::Grabbed; }
    else if (overedGameObject)
    {
        isMouseOver = overedGameObject == this ||
                      overedGameObject->IsChildOf(this);
        selectionState = IsBeingGrabbed() ? SelectionState::Grabbed :
                     (isMouseOver ? SelectionState::Over : SelectionState::Idle);
    }
    SetColor(selectionState);

    // Change being grabbed
    bool prevGrab = IsBeingGrabbed();
    if (!IsBeingGrabbed())
    {
        m_isBeingGrabbed = isMouseOver &&
                         Input::GetMouseButtonDown(MouseButton::Left);
    }
    else
    {
        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            m_isBeingGrabbed = false;
        }
    }
    m_grabHasJustChanged = (IsBeingGrabbed() != prevGrab);
}

void TransformGizmoAxis::SetAxis(Axis3D axis)
{
    if (axis != GetAxis())
    {
        m_axis = axis;
        SetColor(SelectionState::Idle);
    }
}
Axis3D TransformGizmoAxis::GetAxis() const { return m_axis; }
Vector3 TransformGizmoAxis::GetAxisVectorLocal() const
{
    return GetVectorFromAxis( GetAxis() );
}

Vector3 TransformGizmoAxis::GetAxisVectorWorld() const
{
    Transform *refGoT = GetReferencedGameObject()->GetTransform();
    return refGoT->FromLocalToWorldDirection( GetAxisVectorLocal() ).Normalized();
}

bool TransformGizmoAxis::IsBeingGrabbed() const
{
    return m_isBeingGrabbed;
}

bool TransformGizmoAxis::GrabHasJustChanged() const
{
    return m_grabHasJustChanged;
}

void TransformGizmoAxis::SetColor(SelectionState state)
{
    switch (state)
    {
        case SelectionState::Idle:
            {
                Color axisColor = GetColorFromAxis( GetAxis() );
                SetColor(axisColor);
            }
            break;

        case SelectionState::Over:
            SetColor(Color::Orange);
            break;

        case SelectionState::Grabbed:
            SetColor(Color::Yellow);
            break;
    }
}
