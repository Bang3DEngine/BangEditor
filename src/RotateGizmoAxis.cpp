#include "BangEditor/RotateGizmoAxis.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RotateGizmoAxis::RotateGizmoAxis()
{
    SetName("RotateGizmoAxis");
}

RotateGizmoAxis::~RotateGizmoAxis()
{
}

void RotateGizmoAxis::Update()
{
    GameObject::Update();
}

void RotateGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
}

void RotateGizmoAxis::SetColor(const Color &color)
{
}
