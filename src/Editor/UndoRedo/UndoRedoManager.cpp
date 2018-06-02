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

    for (UndoRedoAction *action : urm->m_undoActions)
    {
        delete action;
    }
    for (UndoRedoAction *action : urm->m_redoActions)
    {
        delete action;
    }
    urm->m_undoActions.Clear();
    urm->m_redoActions.Clear();
}

void UndoRedoManager::PushAction(UndoRedoAction *action)
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance();
    ASSERT(urm);
    ASSERT(!urm->m_undoingOrRedoing);

    urm->m_undoActions.PushFront(action);
    urm->m_redoActions.Clear();

    if (urm->m_undoActions.Size() > UndoListSize)
    {
        delete urm->m_undoActions.Back();
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
        UndoRedoAction *action = m_undoActions.Front();

        m_undoingOrRedoing = true;
        action->Undo();
        m_undoingOrRedoing = false;

        m_undoActions.PopFront();
        m_redoActions.PushFront(action);

        if (m_redoActions.Size() > UndoListSize)
        {
            delete m_redoActions.Back();
            m_redoActions.PopBack();
        }
    }
    else if (redo && m_redoActions.Size() >= 1)
    {
        UndoRedoAction *action = m_redoActions.Front();

        m_undoingOrRedoing = true;
        action->Redo();
        m_undoingOrRedoing = false;

        m_redoActions.PopFront();
        m_undoActions.PushFront(action);
    }
}

