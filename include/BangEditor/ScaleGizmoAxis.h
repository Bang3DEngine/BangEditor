#ifndef SCALEGIZMOAXIS_H
#define SCALEGIZMOAXIS_H

#include "BangEditor/TransformGizmoAxis.h"

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ScaleGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(ScaleGizmoAxis);

public:
    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetAxis(Axis3D axis) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    static float ArrowCapScale;

    Vector3 m_startGrabLocalScale = Vector3::Zero;
    Vector3 m_startGrabCenterToMousePointProjLocalV = Vector3::Zero;

    GameObject *p_arrowCap = nullptr;
    GameObject *p_selectionGo = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    MeshRenderer *p_meshRenderer = nullptr;
    MeshRenderer *p_selectionRenderer = nullptr;

    ScaleGizmoAxis();
    virtual ~ScaleGizmoAxis();

    void UpdatePoints(float localAxisLength);
    void SetColor(const Color &color) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCALEGIZMOAXIS_H

