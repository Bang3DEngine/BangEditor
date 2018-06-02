#ifndef UNDOREDOFILECHANGE_H
#define UNDOREDOFILECHANGE_H

#include "Bang/Bang.h"
#include "Bang/Path.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoFileChange : public UndoRedoAction
{
public:
    UndoRedoFileChange(const Path &path);
	virtual ~UndoRedoFileChange();

    void ReadBefore();
    void ReadAfter();

    void Undo() override;
    void Redo() override;

private:
    Path m_path;

    Byte *m_previousContents = nullptr;
    std::size_t m_previousContentsSize = -1;

    Byte *m_newContents = nullptr;
    std::size_t m_newContentsSize = -1;

    void Read(Byte **bytePointer, std::size_t *sizePointer);
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOFILECHANGE_H

