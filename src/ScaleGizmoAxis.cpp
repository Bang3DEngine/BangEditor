#include "BangEditor/ScaleGizmoAxis.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ScaleGizmoAxis::ScaleGizmoAxis()
{
    SetName("ScaleGizmoAxis");
}

ScaleGizmoAxis::~ScaleGizmoAxis()
{
}

void ScaleGizmoAxis::Update()
{
    TransformGizmoAxis::Update();
}

void ScaleGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);
}

void ScaleGizmoAxis::SetColor(const Color &color)
{

}
