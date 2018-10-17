#include "BangEditor/UndoRedoMoveGameObject.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/Editor.h"

using namespace Bang;
using namespace BangEditor;

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

bool UndoRedoMoveGameObject::IsRedundant() const
{
    return (p_previousParent == p_newParent) &&
           (m_indexInPreviousParent == m_indexInNewParent);
}

void UndoRedoMoveGameObject::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    p_movedGameObject = nullptr;
    p_previousParent = nullptr;
    p_newParent = nullptr;
}
