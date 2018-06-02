#include "BangEditor/UndoRedoManager.h"

#include "Bang/Input.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoManager::UndoRedoManager()
{
    ShortcutManager::RegisterShortcut(
        Shortcut(Key::LCTRL, Key::Z, "Undo", true),
        [this](const Shortcut&)
        {
            OnUndoRedoPressed(true);
        });

    ShortcutManager::RegisterShortcut(
        Shortcut(Key::LCTRL, Key::LSHIFT, Key::Z, "Redo", true),
        [this](const Shortcut&)
        {
            OnUndoRedoPressed(false);
        });
}

UndoRedoManager::~UndoRedoManager()
{
}

void UndoRedoManager::Clear()
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance();
    ASSERT(urm);

    for (const Array<UndoRedoAction*> &actions : urm->m_undoActions)
    {
        for (UndoRedoAction *action : actions)
        {
            delete action;
        }
    }

    for (const Array<UndoRedoAction*> &actions : urm->m_redoActions)
    {
        for (UndoRedoAction *action : actions)
        {
            delete action;
        }
    }

    urm->m_undoActions.Clear();
    urm->m_redoActions.Clear();
}

void UndoRedoManager::PushAction(UndoRedoAction *action)
{
    PushActionsInSameStep( {action} );
}

void UndoRedoManager::PushActionsInSameStep(
                            const Array<UndoRedoAction*> &actionsInSameStep)
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance();
    ASSERT(urm);
    ASSERT(!urm->m_undoingOrRedoing);

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

    // Debug_Peek(urm->m_undoActions);
    // Debug_Peek(urm->m_redoActions);
}

UndoRedoManager *UndoRedoManager::GetInstance()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    EditorScene *es = esm ? esm->GetEditorScene() : nullptr;
    return es ? es->GetUndoRedoManager() : nullptr;
}

void UndoRedoManager::OnUndoRedoPressed(bool undo)
{
    if (ScenePlayer::GetPlayState() != PlayState::EDITING)
    {
        return;
    }

    const bool redo = !undo;

    // Debug_Peek(m_undoActions);
    if (undo && m_undoActions.Size() >= 1)
    {
        const Array<UndoRedoAction*> &actions = m_undoActions.Front();

        m_undoingOrRedoing = true;
        for (UndoRedoAction *action : actions)
        {
            action->Undo();
        }
        m_undoingOrRedoing = false;

        m_redoActions.PushFront(actions);
        m_undoActions.PopFront();

        if (m_redoActions.Size() > UndoListSize)
        {
            for (UndoRedoAction *action : m_redoActions.Back())
            {
                delete action;
            }
            m_redoActions.PopBack();
        }
    }
    else if (redo && m_redoActions.Size() >= 1)
    {
        const Array<UndoRedoAction*> &actions = m_redoActions.Front();

        m_undoingOrRedoing = true;
        for (UndoRedoAction *action : actions)
        {
            action->Redo();
        }
        m_undoingOrRedoing = false;

        m_undoActions.PushFront(actions);
        m_redoActions.PopFront();
    }
}

