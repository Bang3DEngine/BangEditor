#ifndef SCALEGIZMO_H
#define SCALEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang {
class GameObject;
}  // namespace Bang

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ScaleGizmoAxis;

class ScaleGizmo : public SelectionGizmo
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

