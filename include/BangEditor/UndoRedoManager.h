#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/List.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ShortcutManager.h"
#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
namespace BangEditor
{
class UndoRedoAction;

class IEventsUndoRedo
{
    IEVENTS_NS(IEventsUndoRedo);

public:
    virtual void OnActionPushed(UndoRedoAction *action)
    {
        BANG_UNUSED(action);
    }
    virtual void OnUndo(UndoRedoAction *action)
    {
        BANG_UNUSED(action);
    }
    virtual void OnRedo(UndoRedoAction *action)
    {
        BANG_UNUSED(action);
    }
};

class UndoRedoManager : public EventEmitter<IEventsUndoRedo>
{
public:
    UndoRedoManager();
    virtual ~UndoRedoManager() override;

    static void Undo();
    static void Redo();
    static void Clear();
    static bool CanUndo();
    static bool CanRedo();
    static void PushAction(UndoRedoAction *action);
    static void PushActionsInSameStep(
        const Array<UndoRedoAction *> &actionsInSameStep);

    static UndoRedoManager *GetInstance();

private:
    static constexpr int UndoListSize = 10000;

    List<Array<UndoRedoAction *>> m_undoActions;
    List<Array<UndoRedoAction *>> m_redoActions;

    // Only for debugging
    bool m_undoingOrRedoing = false;
};
}

#endif  // UNDOREDOMANAGER_H
