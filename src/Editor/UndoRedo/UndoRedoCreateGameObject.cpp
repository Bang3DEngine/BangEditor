#include "BangEditor/UndoRedoCreateGameObject.h"

#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoCreateGameObject::UndoRedoCreateGameObject(GameObject *createdGameObject)
{
    p_createdGameObject = createdGameObject;
    p_parent = createdGameObject->GetParent();
    m_indexInParent = p_parent->GetChildren().IndexOf(createdGameObject);

    p_parent->EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_createdGameObject->EventEmitter<IEventsDestroy>::RegisterListener(this);
}

UndoRedoCreateGameObject::~UndoRedoCreateGameObject()
{
    if (p_createdGameObject && p_createdGameObject->GetParent() == nullptr)
    {
        GameObject::Destroy(p_createdGameObject);
    }
}

void UndoRedoCreateGameObject::Undo()
{
    if (p_createdGameObject)
    {
        p_createdGameObject->SetParent(nullptr);
    }
}

void UndoRedoCreateGameObject::Redo()
{
    if (p_createdGameObject)
    {
        p_createdGameObject->SetParent(p_parent, m_indexInParent);
        Editor::SelectGameObject(p_createdGameObject, false);
    }
}

void UndoRedoCreateGameObject::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_createdGameObject = nullptr;
    p_parent = nullptr;
}
