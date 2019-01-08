#ifndef ROTATEGIZMO_H
#define ROTATEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class GameObject;
class LineRenderer;
class MeshRenderer;
}

using namespace Bang;
namespace BangEditor
{
class RotateGizmoAxis;

class RotateGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(RotateGizmo);

public:
    RotateGizmo();

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

    virtual ~RotateGizmo() override;

    void CreateSphereBoundsPoints();

    friend class RotateGizmoAxis;
};
}

#endif  // ROTATEGIZMO_H
