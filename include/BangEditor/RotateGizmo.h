#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RotateGizmoAxis;

class RotateGizmo : public GameObject,
                    public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RotateGizmo);

public:
    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    RotateGizmoAxis *p_axisX = nullptr;
    RotateGizmoAxis *p_axisY = nullptr;
    RotateGizmoAxis *p_axisZ = nullptr;

	RotateGizmo();
	virtual ~RotateGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // ROTATEGIZMO_H

