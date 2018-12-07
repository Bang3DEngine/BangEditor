#ifndef SCALEGIZMOAXIS_H
#define SCALEGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"
#include "BangEditor/TransformGizmoAxis.h"

namespace Bang
{
class Color;
class GameObject;
class LineRenderer;
class MeshRenderer;
}

using namespace Bang;
namespace BangEditor
{
class ScaleGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(ScaleGizmoAxis);

public:
    ScaleGizmoAxis();

    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetAxis(Axis3DExt axis) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    static float ArrowCapScale;

    Vector3 m_startGrabLocalScale = Vector3::Zero();
    Vector3 m_startGrabCenterToMousePointProjLocalV = Vector3::Zero();

    GameObject *p_arrowCap = nullptr;
    GameObject *p_selectionGo = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    MeshRenderer *p_meshRenderer = nullptr;
    MeshRenderer *p_selectionRenderer = nullptr;

    virtual ~ScaleGizmoAxis() override;

    void UpdatePoints(float localAxisLength);

    // SelectionGizmo
    virtual GameObject *GetSelectionGameObject() const override;

    // TransformGizmoAxis
    bool ApplyAlignmentAlpha() const override;
    void SetColor(const Color &color) override;
};
}

#endif  // SCALEGIZMOAXIS_H
