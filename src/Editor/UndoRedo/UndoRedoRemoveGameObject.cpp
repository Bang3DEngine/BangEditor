#include "BangEditor/UndoRedoRemoveGameObject.h"

#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoRemoveGameObject::UndoRedoRemoveGameObject(GameObject *removedGameObject)
{
    p_removedGameObject = removedGameObject;
    p_previousParent = p_removedGameObject->GetParent();
    m_indexInPreviousParent = p_previousParent->GetChildren().
                              IndexOf(p_removedGameObject);
}

UndoRedoRemoveGameObject::~UndoRedoRemoveGameObject()
{
    if (!p_removedGameObject->GetParent())
    {
        GameObject::Destroy( p_removedGameObject );
    }
}

void UndoRedoRemoveGameObject::Undo()
{
    p_removedGameObject->SetParent(p_previousParent, m_indexInPreviousParent);
    Editor::SelectGameObject(p_removedGameObject, false);
}

void UndoRedoRemoveGameObject::Redo()
{
    p_removedGameObject->SetParent(nullptr);
}
