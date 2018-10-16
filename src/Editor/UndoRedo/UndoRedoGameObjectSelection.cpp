#include "BangEditor/UndoRedoGameObjectSelection.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoGameObjectSelection::UndoRedoGameObjectSelection(
        GameObject *previousSelectedGameObject,
        GameObject *newSelectedGameObject)
{
    p_previousSelectedGameObject = previousSelectedGameObject;
    if (p_previousSelectedGameObject)
    {
        p_previousSelectedGameObject->EventEmitter<IEventsDestroy>::
                                      RegisterListener(this);
    }

    p_newSelectedGameObject = newSelectedGameObject;
    if (p_newSelectedGameObject)
    {
        p_newSelectedGameObject->EventEmitter<IEventsDestroy>::
                                      RegisterListener(this);
    }
}

UndoRedoGameObjectSelection::~UndoRedoGameObjectSelection()
{
}

void UndoRedoGameObjectSelection::Undo()
{
    if (p_previousSelectedGameObject)
    {
        Editor::GetInstance()->SelectGameObject(p_previousSelectedGameObject, false);
    }
}

void UndoRedoGameObjectSelection::Redo()
{
    if (p_newSelectedGameObject)
    {
        Editor::GetInstance()->SelectGameObject(p_newSelectedGameObject, false);
    }
}

bool UndoRedoGameObjectSelection::IsRedundant() const
{
    return (p_previousSelectedGameObject == p_newSelectedGameObject);
}

void UndoRedoGameObjectSelection::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (object == p_previousSelectedGameObject)
    {
        p_previousSelectedGameObject = nullptr;
    }

    if (object == p_newSelectedGameObject)
    {
        p_newSelectedGameObject = nullptr;
    }
}

