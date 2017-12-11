#ifndef TRANSFORMGIZMO_H
#define TRANSFORMGIZMO_H

#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TranslateGizmo;

class TransformGizmo : public GameObject,
                       public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TransformGizmo);

public:
    // GameObject
    void Update() override;

private:
    TranslateGizmo *p_translateGizmo = nullptr;

	TransformGizmo();
    virtual ~TransformGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // TRANSFORMGIZMO_H

