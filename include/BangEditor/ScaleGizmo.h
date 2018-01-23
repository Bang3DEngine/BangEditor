#ifndef SCALEGIZMO_H
#define SCALEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ScaleGizmoAxis;

class ScaleGizmo : public GameObject,
                   public SelectionGizmo
{
    GAMEOBJECT_EDITOR(ScaleGizmo);

public:
    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    ScaleGizmoAxis *p_axisX   = nullptr;
    ScaleGizmoAxis *p_axisY   = nullptr;
    ScaleGizmoAxis *p_axisZ   = nullptr;
    ScaleGizmoAxis *p_axisXYZ = nullptr;

    ScaleGizmo();
    virtual ~ScaleGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // SCALEGIZMO_H

