#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
using namespace BangEditor;

UndoRedoAction::UndoRedoAction()
{
}

UndoRedoAction::~UndoRedoAction()
{
}

bool UndoRedoAction::IsRedundant() const
{
    return false;
}
