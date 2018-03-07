#ifndef ROTATEGIZMOAXIS_H
#define ROTATEGIZMOAXIS_H

#include "Bang/ResourceHandle.h"

#include "BangEditor/TransformGizmoAxis.h"

NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD class LineRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RotateGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(RotateGizmoAxis);

public:
    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetAxis(Axis3DExt axis) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Vector2 m_startingGrabMousePosNDC;
    Vector3 m_startingGrabAxisedSpherePoint;

    RH<Mesh> m_selectionMesh;
    GameObject *p_selectionGo = nullptr;
    MeshRenderer *p_selectionRenderer = nullptr;

    LineRenderer *p_circleRenderer = nullptr;

    RotateGizmoAxis();
    virtual ~RotateGizmoAxis();

    void UpdateCirclePoints();
    static Quaternion GetQuaternionAxised(const Quaternion &q, Axis3DExt axis);

    void SetColor(const Color &color) override;

};

NAMESPACE_BANG_EDITOR_END

#endif // ROTATEGIZMOAXIS_H

