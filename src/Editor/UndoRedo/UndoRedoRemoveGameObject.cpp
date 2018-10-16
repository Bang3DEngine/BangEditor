#include "BangEditor/UndoRedoRemoveGameObject.h"

#include "Bang/Array.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoRemoveGameObject::UndoRedoRemoveGameObject(GameObject *removedGameObject)
{
    p_removedGameObject = removedGameObject;
    if (p_removedGameObject)
    {
        p_removedGameObject->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }

    p_previousParent = p_removedGameObject->GetParent();
    m_indexInPreviousParent = p_previousParent->GetChildren().
                              IndexOf(p_removedGameObject);
    if (p_previousParent)
    {
        p_previousParent->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }
}

UndoRedoRemoveGameObject::~UndoRedoRemoveGameObject()
{
    if (p_removedGameObject && !p_removedGameObject->GetParent())
    {
        GameObject::Destroy( p_removedGameObject );
    }
}

void UndoRedoRemoveGameObject::Undo()
{
    if (p_removedGameObject)
    {
        p_removedGameObject->SetParent(p_previousParent, m_indexInPreviousParent);
        Editor::SelectGameObject(p_removedGameObject, false);
    }
}

void UndoRedoRemoveGameObject::Redo()
{
    if (p_removedGameObject)
    {
        p_removedGameObject->SetParent(nullptr);
    }
}

void UndoRedoRemoveGameObject::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_removedGameObject = nullptr;
    p_previousParent = nullptr;
}
