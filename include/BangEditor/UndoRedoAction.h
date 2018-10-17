#ifndef UNDOREDOACTION_H
#define UNDOREDOACTION_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // UNDOREDOACTION_H
