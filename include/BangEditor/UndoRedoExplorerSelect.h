#ifndef UNDOREDOEXPLORERSELECT_H
#define UNDOREDOEXPLORERSELECT_H

#include "Bang/Bang.h"
#include "Bang/Path.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoExplorerSelect : public UndoRedoAction
{
public:
    UndoRedoExplorerSelect(const Path &previousSelectedPath,
                           const Path &newSelectedPath);
	virtual ~UndoRedoExplorerSelect();

    // UndoRedoAction
    void Undo() override;
    void Redo() override;

private:
    Path m_previousSelectedPath = Path::Empty;
    Path m_newSelectedPath = Path::Empty;

    void RestorePath(const Path &path);
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOEXPLORERSELECT_H
