#ifndef TRANSLATEGIZMO_H
#define TRANSLATEGIZMO_H

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
class TranslateGizmoAxis;

class TranslateGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TranslateGizmo);

public:
    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    TranslateGizmoAxis *p_axisX = nullptr;
    TranslateGizmoAxis *p_axisY = nullptr;
    TranslateGizmoAxis *p_axisZ = nullptr;

    TranslateGizmo();
    virtual ~TranslateGizmo() override;
};
}

#endif  // TRANSLATEGIZMO_H
