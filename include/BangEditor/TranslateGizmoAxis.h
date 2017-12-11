#ifndef TRANSLATEGIZMOAXIS_H
#define TRANSLATEGIZMOAXIS_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class TranslateGizmoAxis : public GameObject,
                           public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TranslateGizmoAxis);

public:
    void SetAxis(Axis3D axis);

    Axis3D GetAxis() const;
    Vector3 GetAxisVector() const;

private:
    Axis3D m_axis = Undef<Axis3D>();

    TranslateGizmoAxis();
    virtual ~TranslateGizmoAxis();
};


NAMESPACE_BANG_EDITOR_END

#endif // TRANSLATEGIZMOAXIS_H

