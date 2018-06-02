#include "BangEditor/UndoRedoCreateGameObject.h"

#include "Bang/GameObject.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoCreateGameObject::UndoRedoCreateGameObject(GameObject *createdGameObject)
{
    p_createdGameObject = createdGameObject;
    p_parent = createdGameObject->GetParent();
    m_indexInParent = p_parent->GetChildren().IndexOf(createdGameObject);
}

UndoRedoCreateGameObject::~UndoRedoCreateGameObject()
{
    if (p_createdGameObject->GetParent() == nullptr)
    {
        GameObject::Destroy(p_createdGameObject);
    }
}

void UndoRedoCreateGameObject::Undo()
{
    p_createdGameObject->SetParent(nullptr);
}

void UndoRedoCreateGameObject::Redo()
{
    p_createdGameObject->SetParent(p_parent, m_indexInParent);
}
