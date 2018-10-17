#include "BangEditor/UndoRedoManager.h"

#include "Bang/Assert.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/Flags.h"
#include "Bang/Key.h"
#include "Bang/List.tcc"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/Shortcut.h"
#include "BangEditor/ShortcutManager.h"
#include "BangEditor/UndoRedoAction.h"

using namespace Bang;
using namespace BangEditor;

UndoRedoManager::UndoRedoManager()
{
    ShortcutManager::RegisterShortcut(
        Shortcut(Key::Z, KeyModifier::LCTRL, "Undo", true),
        [](const Shortcut &) { UndoRedoManager::Undo(); });

    ShortcutManager::RegisterShortcut(
        Shortcut(
            Key::Z, (KeyModifier::LCTRL | KeyModifier::LSHIFT), "Redo", true),
        [](const Shortcut &) { UndoRedoManager::Redo(); });
}

UndoRedoManager::~UndoRedoManager()
{
}

void UndoRedoManager::Undo()
{
    if (UndoRedoManager::CanUndo())
    {
        UndoRedoManager *urm = UndoRedoManager::GetInstance();
        const Array<UndoRedoAction *> &actions = urm->m_undoActions.Front();

        urm->m_undoingOrRedoing = true;
        for (UndoRedoAction *action : actions)
        {
            action->Undo();
            urm->EventEmitter<IEventsUndoRedo>::PropagateToListeners(
                &IEventsUndoRedo::OnUndo, action);
        }
        urm->m_undoingOrRedoing = false;

        urm->m_redoActions.PushFront(actions);
        urm->m_undoActions.PopFront();

        if (urm->m_redoActions.Size() > UndoRedoManager::UndoListSize)
        {
            for (UndoRedoAction *action : urm->m_redoActions.Back())
            {
                delete action;
            }
            urm->m_redoActions.PopBack();
        }
    }
}

void UndoRedoManager::Redo()
{
    if (UndoRedoManager::CanRedo())
    {
        UndoRedoManager *urm = UndoRedoManager::GetInstance();
        const Array<UndoRedoAction *> &actions = urm->m_redoActions.Front();

        urm->m_undoingOrRedoing = true;
        for (UndoRedoAction *action : actions)
        {
            action->Redo();
            urm->EventEmitter<IEventsUndoRedo>::PropagateToListeners(
                &IEventsUndoRedo::OnRedo, action);
        }
        urm->m_undoingOrRedoing = false;

        urm->m_undoActions.PushFront(actions);
        urm->m_redoActions.PopFront();
    }
}

void UndoRedoManager::Clear()
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance();
    ASSERT(urm);

    for (const Array<UndoRedoAction *> &actions : urm->m_undoActions)
    {
        for (UndoRedoAction *action : actions)
        {
            delete action;
        }
    }

    for (const Array<UndoRedoAction *> &actions : urm->m_redoActions)
    {
        for (UndoRedoAction *action : actions)
        {
            delete action;
        }
    }

    urm->m_undoActions.Clear();
    urm->m_redoActions.Clear();
}

bool UndoRedoManager::CanUndo()
{
    return ScenePlayer::GetPlayState() == PlayState::EDITING &&
           UndoRedoManager::GetInstance()->m_undoActions.Size() >= 1;
}

bool UndoRedoManager::CanRedo()
{
    return ScenePlayer::GetPlayState() == PlayState::EDITING &&
           UndoRedoManager::GetInstance()->m_redoActions.Size() >= 1;
}

void UndoRedoManager::PushAction(UndoRedoAction *action)
{
    PushActionsInSameStep({action});
}

void UndoRedoManager::PushActionsInSameStep(
    const Array<UndoRedoAction *> &actionsInSameStep)
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance();
    ASSERT(urm);
    ASSERT(!urm->m_undoingOrRedoing);

    bool allStepsRedundant = true;
    for (UndoRedoAction *pushedAction : actionsInSameStep)
    {
        if (!pushedAction->IsRedundant())
        {
            allStepsRedundant = false;
            break;
        }
    }

    if (!allStepsRedundant)
    {
        for (UndoRedoAction *pushedAction : actionsInSameStep)
        {
            urm->EventEmitter<IEventsUndoRedo>::PropagateToListeners(
                &IEventsUndoRedo::OnActionPushed, pushedAction);
        }

        urm->m_undoActions.PushFront(actionsInSameStep);
        urm->m_redoActions.Clear();

        if (urm->m_undoActions.Size() > UndoListSize)
        {
            for (UndoRedoAction *action : urm->m_undoActions.Back())
            {
                delete action;
            }
            urm->m_undoActions.PopBack();
        }
    }
}

UndoRedoManager *UndoRedoManager::GetInstance()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    EditorScene *es = esm ? esm->GetEditorScene() : nullptr;
    return es ? es->GetUndoRedoManager() : nullptr;
}
