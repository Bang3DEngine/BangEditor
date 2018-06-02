#include "BangEditor/UndoRedoMoveGameObject.h"

#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoMoveGameObject::UndoRedoMoveGameObject(GameObject *movedGameObject,
                                               GameObject *previousParent,
                                               int indexInPreviousParent,
                                               GameObject *newParent,
                                               int indexInNewParent)
{
    p_movedGameObject = movedGameObject;
    if (p_movedGameObject)
    {
        p_movedGameObject->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }

    p_previousParent = previousParent;
    m_indexInPreviousParent = indexInPreviousParent;
    if (p_previousParent)
    {
        p_previousParent->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }

    p_newParent = newParent;
    m_indexInNewParent = indexInNewParent;
    if (p_newParent)
    {
        p_newParent->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }
}

UndoRedoMoveGameObject::~UndoRedoMoveGameObject()
{
}

void UndoRedoMoveGameObject::Undo()
{
    if (p_movedGameObject)
    {
        p_movedGameObject->SetParent(p_previousParent, m_indexInPreviousParent);
        Editor::SelectGameObject(p_movedGameObject, false);
    }
}

void UndoRedoMoveGameObject::Redo()
{
    if (p_movedGameObject)
    {
        p_movedGameObject->SetParent(p_newParent, m_indexInNewParent);
        Editor::SelectGameObject(p_movedGameObject, false);
    }
}

void UndoRedoMoveGameObject::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_movedGameObject = nullptr;
    p_previousParent = nullptr;
    p_newParent = nullptr;
}

