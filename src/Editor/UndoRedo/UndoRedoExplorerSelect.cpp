#include "BangEditor/UndoRedoExplorerSelect.h"

#include "BangEditor/Explorer.h"

using namespace Bang;
using namespace BangEditor;

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

bool UndoRedoExplorerSelect::IsRedundant() const
{
    return (m_previousSelectedPath == m_newSelectedPath);
}

void UndoRedoExplorerSelect::RestorePath(const Path &path)
{
    Explorer *explorer = Explorer::GetInstance();
    explorer->SelectPath(path, false);
}
