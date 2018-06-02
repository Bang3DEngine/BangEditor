#include "BangEditor/UndoRedoMoveGameObject.h"

#include "Bang/GameObject.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoMoveGameObject::UndoRedoMoveGameObject(GameObject *movedGameObject,
                                               GameObject *previousParent,
                                               int indexInPreviousParent,
                                               GameObject *newParent,
                                               int indexInNewParent)
{
    p_movedGameObject = movedGameObject;

    p_previousParent = previousParent;
    m_indexInPreviousParent = indexInPreviousParent;

    p_newParent = newParent;
    m_indexInNewParent = indexInNewParent;
}

UndoRedoMoveGameObject::~UndoRedoMoveGameObject()
{
}

void UndoRedoMoveGameObject::Undo()
{
    p_movedGameObject->SetParent(p_previousParent, m_indexInPreviousParent);
}

void UndoRedoMoveGameObject::Redo()
{
    p_movedGameObject->SetParent(p_newParent, m_indexInNewParent);
}

