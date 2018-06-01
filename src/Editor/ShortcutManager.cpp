#include "BangEditor/ShortcutManager.h"

#include "BangEditor/EditorWindow.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void ShortcutManager::RegisterShortcut(const Shortcut &shortcut,
                                       ShortcutManager::ShortcutCallback callback)
{
    ShortcutManager *sm = ShortcutManager::GetInstance();
    if (!sm->m_shortcuts.ContainsKey(shortcut))
    {
        sm->m_shortcuts.Add(shortcut, Array<ShortcutCallback>());
    }
    sm->m_shortcuts.Get(shortcut).PushBack(callback);
}

ShortcutManager::ShortcutManager()
{
}

ShortcutManager::~ShortcutManager()
{
}

void ShortcutManager::Update()
{
    if (!Input::GetActive()) { return; }

    // Process pressed keys
    const Array<Key> pressedKeys = Input::GetPressedKeys();
    const Array<Key> keysDown = Input::GetKeysDown();
    const Array<Key> keysDownRepeat = Input::GetKeysDownRepeat();
    for (const auto &pair : m_shortcuts)
    {
        // Trigger callbacks
        const Shortcut &shortcut = pair.first;
        if (shortcut.IsTriggered(pressedKeys, keysDown, keysDownRepeat))
        {
            const Array<ShortcutCallback> &shortcutCallbacks = pair.second;
            for (ShortcutCallback cb : shortcutCallbacks) { cb(shortcut); }
        }
    }
}

ShortcutManager *ShortcutManager::GetInstance()
{
    return EditorWindow::GetActive()->GetShortcutManager();
}


