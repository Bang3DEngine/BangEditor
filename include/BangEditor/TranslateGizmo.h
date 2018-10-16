#ifndef TRANSLATEGIZMO_H
#define TRANSLATEGIZMO_H

#include "Bang/Axis.h"
#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class GameObject;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TranslateGizmoAxis;

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


NAMESPACE_BANG_EDITOR_END

#endif // TRANSLATEGIZMO_H

