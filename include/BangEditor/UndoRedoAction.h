#ifndef UNDOREDOACTION_H
#define UNDOREDOACTION_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoAction
{
public:
    virtual ~UndoRedoAction();

    virtual void Undo() = 0;
    virtual void Redo() = 0;

protected:
	UndoRedoAction();
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOACTION_H

