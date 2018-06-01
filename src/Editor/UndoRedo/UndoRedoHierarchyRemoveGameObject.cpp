#include "BangEditor/UndoRedoHierarchyRemoveGameObject.h"

#include "Bang/GameObject.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoHierarchyRemoveGameObject::
            UndoRedoHierarchyRemoveGameObject(GameObject *removedGameObject)
{
    p_removedGameObject = removedGameObject;
    p_previousParent = p_removedGameObject->GetParent();
    m_indexInPreviousParent = p_previousParent->GetChildren().
                              IndexOf(p_removedGameObject);
}

UndoRedoHierarchyRemoveGameObject::~UndoRedoHierarchyRemoveGameObject()
{
    if (!p_removedGameObject->GetParent())
    {
        GameObject::Destroy( p_removedGameObject );
    }
}

void UndoRedoHierarchyRemoveGameObject::Undo()
{
    p_removedGameObject->SetParent(p_previousParent, m_indexInPreviousParent);
}

void UndoRedoHierarchyRemoveGameObject::Redo()
{
    p_removedGameObject->SetParent(nullptr);
}
