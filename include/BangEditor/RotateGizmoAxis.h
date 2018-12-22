#ifndef ROTATEGIZMOAXIS_H
#define ROTATEGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/RenderPass.h"
#include "Bang/AssetHandle.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"
#include "BangEditor/TransformGizmoAxis.h"

namespace Bang
{
class Color;
class GameObject;
class LineRenderer;
class Mesh;
class MeshRenderer;
}

using namespace Bang;
namespace BangEditor
{
class RotateGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR(RotateGizmoAxis);

public:
    RotateGizmoAxis();

    // GameObject
    void Update() override;

    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetAxis(Axis3DExt axis) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Vector2 m_startingGrabMousePosNDC;
    Vector3 m_startingGrabAxisedSpherePoint;

    AH<Mesh> m_selectionMesh;
    GameObject *p_selectionGo = nullptr;
    MeshRenderer *p_selectionRenderer = nullptr;

    LineRenderer *p_circleRenderer = nullptr;

    virtual ~RotateGizmoAxis() override;

    void UpdateCirclePoints();
    static Quaternion GetQuaternionAxised(const Quaternion &q, Axis3DExt axis);

    // SelectionGizmo
    virtual GameObject *GetSelectionGameObject() const override;

    // TransformGizmoAxis
    bool ApplyAlignmentAlpha() const override;
    void SetColor(const Color &color) override;
};
}

#endif  // ROTATEGIZMOAXIS_H
