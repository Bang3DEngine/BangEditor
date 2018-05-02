#ifndef GAMEOBJECTSELECTIONGIZMO_H
#define GAMEOBJECTSELECTIONGIZMO_H

#include "Bang/GameObject.h"
#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class GameObjectSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(GameObjectSelectionGizmo);

public:
    GameObjectSelectionGizmo();
    virtual ~GameObjectSelectionGizmo();

    // GameObject
    void Render(RenderPass rp, bool renderChildren) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // GAMEOBJECTSELECTIONGIZMO_H

