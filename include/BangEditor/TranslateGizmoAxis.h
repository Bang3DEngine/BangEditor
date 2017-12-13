#ifndef TRANSLATEGIZMOAXIS_H
#define TRANSLATEGIZMOAXIS_H

#include "BangEditor/TransformGizmoAxis.h"

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD class MeshRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class TranslateGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(TranslateGizmoAxis);

public:
    // GameObject
    void Update() override;

    void SetAxis(Axis3D axis) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Vector3 m_grabOffset = Vector3::Zero;

    GameObject *p_arrowCap = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    MeshRenderer *p_meshRenderer = nullptr;

    void SetColor(const Color &color) override;

    TranslateGizmoAxis();
    virtual ~TranslateGizmoAxis();
};


NAMESPACE_BANG_EDITOR_END

#endif // TRANSLATEGIZMOAXIS_H

