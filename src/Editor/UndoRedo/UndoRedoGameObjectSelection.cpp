#include "BangEditor/UndoRedoGameObjectSelection.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoGameObjectSelection::UndoRedoGameObjectSelection(
        GameObject *previousSelectedGameObject,
        GameObject *newSelectedGameObject)
{
    p_previousSelectedGameObject = previousSelectedGameObject;
    p_newSelectedGameObject = newSelectedGameObject;
}

UndoRedoGameObjectSelection::~UndoRedoGameObjectSelection()
{
}

void UndoRedoGameObjectSelection::Undo()
{
    Editor::GetInstance()->SelectGameObject_(p_previousSelectedGameObject, false);
}

void UndoRedoGameObjectSelection::Redo()
{
    Editor::GetInstance()->SelectGameObject_(p_newSelectedGameObject, false);
}

