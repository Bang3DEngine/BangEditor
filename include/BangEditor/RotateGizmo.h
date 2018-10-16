#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class GameObject;
FORWARD class LineRenderer;
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RotateGizmoAxis;

class RotateGizmo : public SelectionGizmo
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
	virtual ~RotateGizmo() override;

    void CreateSphereBoundsPoints();

    friend class RotateGizmoAxis;
};

NAMESPACE_BANG_EDITOR_END

#endif // ROTATEGIZMO_H

