#ifndef TRANSLATEGIZMOAXIS_H
#define TRANSLATEGIZMOAXIS_H

#include "BangMath/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"
#include "BangEditor/TransformGizmoAxis.h"

namespace Bang
{
template <typename>
class ColorG;
class GameObject;
class LineRenderer;
class MeshRenderer;
}

using namespace Bang;
namespace BangEditor
{
class TranslateGizmoAxis : public TransformGizmoAxis
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(TranslateGizmoAxis);

public:
    TranslateGizmoAxis();

    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetAxis(Axis3DExt axis) override;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Vector3 m_startGrabPoint = Vector3::Zero();

    GameObject *p_arrowCap = nullptr;
    GameObject *p_selectionGo = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    MeshRenderer *p_meshRenderer = nullptr;
    MeshRenderer *p_selectionRenderer = nullptr;

    // SelectionGizmo
    virtual GameObject *GetSelectionGameObject() const override;

    void SetColor(const Color &color) override;

    virtual ~TranslateGizmoAxis() override;
};
}

#endif  // TRANSLATEGIZMOAXIS_H
