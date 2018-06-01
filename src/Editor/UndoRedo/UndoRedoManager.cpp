#include "BangEditor/UndoRedoManager.h"

#include "Bang/Input.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UndoRedoManager::UndoRedoManager()
{
    ShortcutManager::RegisterShortcut(
        Shortcut(Key::LCTRL, Key::Z, "Undo"),
        [this](const Shortcut&)
        {
            OnUndoRedoPressed(true);
        });

    ShortcutManager::RegisterShortcut(
        Shortcut(Key::LCTRL, Key::LSHIFT, Key::Z, "Redo"),
        [this](const Shortcut&)
        {
            OnUndoRedoPressed(false);
        });
}

UndoRedoManager::~UndoRedoManager()
{
}

void UndoRedoManager::PushAction(UndoRedoAction *action)
{
    UndoRedoManager *urm = UndoRedoManager::GetInstance(); ASSERT(urm);
    urm->m_undoActions.PushFront(action);
    urm->m_redoActions.Clear();
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
    const bool redo = !undo;

    if (undo && m_undoActions.Size() >= 1)
    {
        UndoRedoAction *action = m_undoActions.Front();
        action->Undo();

        m_undoActions.PopFront();
        m_redoActions.PushFront(action);

        if (m_redoActions.Size() >= UndoListSize)
        {
            delete m_redoActions.Back();
            m_redoActions.PopBack();
        }
    }
    else if (redo && m_redoActions.Size() >= 1)
    {
        UndoRedoAction *action = m_redoActions.Front();
        action->Redo();

        m_redoActions.PopFront();
        m_undoActions.PushFront(action);

        if (m_undoActions.Size() >= UndoListSize)
        {
            delete m_undoActions.Back();
            m_undoActions.PopBack();
        }
    }
}

