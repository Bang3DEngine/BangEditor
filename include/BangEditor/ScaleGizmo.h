#ifndef SCALEGIZMO_H
#define SCALEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class GameObject;
}

using namespace Bang;
namespace BangEditor
{
class ScaleGizmoAxis;

class ScaleGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(ScaleGizmo);

public:
    ScaleGizmo();

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    ScaleGizmoAxis *p_axisX = nullptr;
    ScaleGizmoAxis *p_axisY = nullptr;
    ScaleGizmoAxis *p_axisZ = nullptr;
    ScaleGizmoAxis *p_axisXYZ = nullptr;

    virtual ~ScaleGizmo() override;
};
}

#endif  // SCALEGIZMO_H
