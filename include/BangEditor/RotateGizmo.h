#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class LineRenderer;
FORWARD class MeshRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RotateGizmoAxis;

class RotateGizmo : public GameObject,
                    public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RotateGizmo);

public:
    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    RotateGizmoAxis *p_axisX = nullptr;
    RotateGizmoAxis *p_axisY = nullptr;
    RotateGizmoAxis *p_axisZ = nullptr;

    GameObject *p_sphereGo = nullptr;
    GameObject *p_sphereBoundsGo = nullptr;
    MeshRenderer *p_sphereRenderer = nullptr;
    LineRenderer *p_sphereBoundsRenderer = nullptr;

    RotateGizmo();
	virtual ~RotateGizmo();

    void CreateSphereBoundsPoints();

    friend class RotateGizmoAxis;
};

NAMESPACE_BANG_EDITOR_END

#endif // ROTATEGIZMO_H

