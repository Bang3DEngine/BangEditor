#ifndef UNDOREDOACTION_H
#define UNDOREDOACTION_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoAction
{
public:
    virtual ~UndoRedoAction();

    virtual void Undo() = 0;
    virtual void Redo() = 0;

    // Whether the undo/redo does not make any real change
    virtual bool IsRedundant() const;

protected:
	UndoRedoAction();
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOACTION_H

