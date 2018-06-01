#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"
#include "BangEditor/ShortcutManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UndoRedoManager
{
public:
	UndoRedoManager();
	virtual ~UndoRedoManager();

    static void PushAction(UndoRedoAction *action);

    static UndoRedoManager* GetInstance();

private:
    const int UndoListSize = 1000;
    List<UndoRedoAction*> m_undoActions;
    List<UndoRedoAction*> m_redoActions;

    void OnUndoRedoPressed(bool undo);
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOMANAGER_H

