#ifndef GAMEOBJECTSELECTIONGIZMO_H
#define GAMEOBJECTSELECTIONGIZMO_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

using namespace Bang;
namespace BangEditor
{
class GameObjectSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(GameObjectSelectionGizmo);

public:
    GameObjectSelectionGizmo();
    virtual ~GameObjectSelectionGizmo() override;

    // GameObject
    void Render(RenderPass rp, bool renderChildren) override;
};
}

#endif  // GAMEOBJECTSELECTIONGIZMO_H
