#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void SelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    p_referencedGameObject = referencedGameObject;
}

GameObject *SelectionGizmo::GetReferencedGameObject() const
{
    return p_referencedGameObject;
}
