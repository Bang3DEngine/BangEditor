#ifndef TRANSLATEGIZMOAXIS_H
#define TRANSLATEGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD class MeshRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class TranslateGizmoAxis : public GameObject,
                           public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TranslateGizmoAxis);

public:
    // GameObject
    void Update() override;

    void SetAxis(Axis3D axis);

    Axis3D GetAxis() const;
    Vector3 GetAxisVectorLocal() const;
    Vector3 GetAxisVectorWorld() const;

private:
    using SelectionState = SelectionGizmo::SelectionState;

    Axis3D m_axis = Undef<Axis3D>();
    Vector3 m_grabOffset = Vector3::Zero;
    bool m_beingGrabbed = false;

    GameObject *p_arrowCap = nullptr;
    LineRenderer *p_lineRenderer = nullptr;
    MeshRenderer *p_meshRenderer = nullptr;

    void SetColor(SelectionState state);

    TranslateGizmoAxis();
    virtual ~TranslateGizmoAxis();
};


NAMESPACE_BANG_EDITOR_END

#endif // TRANSLATEGIZMOAXIS_H

