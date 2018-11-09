#ifndef UNDOREDOEXPLORERSELECT_H
#define UNDOREDOEXPLORERSELECT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
namespace BangEditor
{
class UndoRedoExplorerSelect : public UndoRedoAction
{
public:
    UndoRedoExplorerSelect(const Path &previousSelectedPath,
                           const Path &newSelectedPath);
    virtual ~UndoRedoExplorerSelect() override;

    // UndoRedoAction
    void Undo() override;
    void Redo() override;
    bool IsRedundant() const override;

private:
    Path m_previousSelectedPath = Path::Empty();
    Path m_newSelectedPath = Path::Empty();

    void RestorePath(const Path &path);
};
}  // namespace BangEditor

#endif  // UNDOREDOEXPLORERSELECT_H
