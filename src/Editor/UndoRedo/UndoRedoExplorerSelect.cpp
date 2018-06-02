#include "BangEditor/UndoRedoExplorerSelect.h"

#include "BangEditor/Explorer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoExplorerSelect::UndoRedoExplorerSelect(const Path &previousSelectedPath,
                                               const Path &newSelectedPath)
{
    m_previousSelectedPath = previousSelectedPath;
    m_newSelectedPath = newSelectedPath;
}

UndoRedoExplorerSelect::~UndoRedoExplorerSelect()
{
}

void UndoRedoExplorerSelect::Undo()
{
    RestorePath(m_previousSelectedPath);
}

void UndoRedoExplorerSelect::Redo()
{
    RestorePath(m_newSelectedPath);
}

void UndoRedoExplorerSelect::RestorePath(const Path &path)
{
    Explorer *explorer = Explorer::GetInstance();
    explorer->SelectPath(path, false);
}

