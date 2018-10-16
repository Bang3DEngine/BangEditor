#ifndef TRANSFORMGIZMOAXIS_H
#define TRANSFORMGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang {
class Color;
class Object;
}  // namespace Bang

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD class MeshRenderer;

NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class TransformGizmoAxis : public SelectionGizmo
{
public:
    // GameObject
    void Update() override;

    virtual void SetAxis(Axis3DExt axis);
    virtual void SetColor(const Color &color) = 0;

    Axis3DExt GetAxis() const;
    Vector3 GetAxisVectorLocal() const;
    Vector3 GetAxisVectorWorld() const;

protected:
    TransformGizmoAxis();
    virtual ~TransformGizmoAxis();

    virtual bool ApplyAlignmentAlpha() const;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Axis3DExt m_axis = Undef<Axis3DExt>();

    void SetColor(SelectionState state);

    // IEventsObject
    void OnDisabled(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // TRANSFORMGIZMOAXIS_H

