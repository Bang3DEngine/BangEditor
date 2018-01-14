#include "BangEditor/SelectionGizmo.h"

#include "Bang/GameObject.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void SelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (GetReferencedGameObject() != referencedGameObject)
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        p_referencedGameObject = referencedGameObject;
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->EventEmitter<IDestroyListener>::RegisterListener(this);
        }
    }
}

GameObject *SelectionGizmo::GetReferencedGameObject() const
{
    return p_referencedGameObject;
}

void SelectionGizmo::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    ASSERT( !GetReferencedGameObject() || object == GetReferencedGameObject() );
    SetReferencedGameObject(nullptr);
}
