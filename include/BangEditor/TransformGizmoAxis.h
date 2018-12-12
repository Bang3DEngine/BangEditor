#ifndef TRANSFORMGIZMOAXIS_H
#define TRANSFORMGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class Color;
class LineRenderer;
class MeshRenderer;
class Object;
}

using namespace Bang;
namespace BangEditor
{
class TransformGizmoAxis : public SelectionGizmo
{
public:
    // GameObject
    void Update() override;

    bool IsLocal() const;
    virtual void SetAxis(Axis3DExt axis);
    virtual void SetColor(const Color &color) = 0;

    Axis3DExt GetAxis() const;
    Vector3 GetAxisVectorLocal() const;
    Vector3 GetAxisVectorWorld() const;

protected:
    TransformGizmoAxis();
    virtual ~TransformGizmoAxis() override;

    virtual bool ApplyAlignmentAlpha() const;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Axis3DExt m_axis = Undef<Axis3DExt>();

    void SetColor(SelectionState state);

    // IEventsObject
    void OnDisabled(Object *object) override;
};
}

#endif  // TRANSFORMGIZMOAXIS_H
