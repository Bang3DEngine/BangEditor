#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include "Bang/Bang.h"
#include "Bang/IEvents.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/UndoRedoAction.h"
#include "BangEditor/ShortcutManager.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class IEventsUndoRedo
{
    IEVENTS_NS(IEventsUndoRedo)

public:
    virtual void OnActionPushed(UndoRedoAction *action) { }
    virtual void OnUndo(UndoRedoAction *action) { }
    virtual void OnRedo(UndoRedoAction *action) { }
};

class UndoRedoManager : public EventEmitter<IEventsUndoRedo>
{
public:
	UndoRedoManager();
	virtual ~UndoRedoManager();

    static void Undo();
    static void Redo();
    static void Clear();
    static bool CanUndo();
    static bool CanRedo();
    static void PushAction(UndoRedoAction *action);
    static void PushActionsInSameStep(
                        const Array<UndoRedoAction*> &actionsInSameStep);

    static UndoRedoManager* GetInstance();

private:
    static constexpr int UndoListSize = 10000;

    List< Array<UndoRedoAction*> > m_undoActions;
    List< Array<UndoRedoAction*> > m_redoActions;

    // Only for debugging
    bool m_undoingOrRedoing = false;
};

NAMESPACE_BANG_EDITOR_END

#endif // UNDOREDOMANAGER_H

