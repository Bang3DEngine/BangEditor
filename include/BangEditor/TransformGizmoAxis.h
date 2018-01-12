#ifndef TRANSFORMGIZMOAXIS_H
#define TRANSFORMGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD class MeshRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class TransformGizmoAxis : public GameObject,
                           public SelectionGizmo
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

    bool IsBeingGrabbed() const;
    bool GrabHasJustChanged() const;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Axis3DExt m_axis = Undef<Axis3DExt>();
    bool m_grabHasJustChanged = false;
    bool m_isBeingGrabbed = false;

    void SetColor(SelectionState state);

    // IEnabledListener
    void OnDisabled() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // TRANSFORMGIZMOAXIS_H

