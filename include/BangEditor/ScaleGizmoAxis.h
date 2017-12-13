#ifndef SCALEGIZMOAXIS_H
#define SCALEGIZMOAXIS_H

#include "BangEditor/TransformGizmoAxis.h"

NAMESPACE_BANG_BEGIN
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ScaleGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(ScaleGizmoAxis);

public:
    // GameObject
    void Update() override;

    void SetAxis(Axis3D axis) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    ScaleGizmoAxis();
    virtual ~ScaleGizmoAxis();

    void SetColor(const Color &color) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // SCALEGIZMOAXIS_H

