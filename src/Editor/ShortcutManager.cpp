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
    Array<Key> pressedKeys = Input::GetPressedKeys();
    bool samePressedKeys = true;
    for (Key k : pressedKeys)
    { if (!m_pressedKeys.Contains(k)) { samePressedKeys = false; } }
    for (Key k : m_pressedKeys)
    { if (!pressedKeys.Contains(k)) { samePressedKeys = false; } }

    if (!samePressedKeys)
    {
        m_pressedKeys = pressedKeys;
        m_shortcutsTriggeredWithCurrentKeys.Clear();
    }

    // Call callbacks of triggered shortcuts
    Array<Key> keysDown = Input::GetKeysDown();
    for (const auto &pair : m_shortcuts)
    {
        const Shortcut &shortcut = pair.first;

        // Is some shortcut key down now?
        bool someShortcutKeyIsDownNow = false;
        for (Key k : shortcut.GetKeys())
        {
            if (keysDown.Contains(k))
            {
                someShortcutKeyIsDownNow = true;
                break;
            }
        }

        // Trigger callbacks
        if (someShortcutKeyIsDownNow &&
            !m_shortcutsTriggeredWithCurrentKeys.Contains(shortcut) &&
            shortcut.IsTriggered(pressedKeys))
        {
            m_shortcutsTriggeredWithCurrentKeys.Add(shortcut);

             // Callbacks
            const Array<ShortcutCallback> &shortcutCallbacks = pair.second;
            for (ShortcutCallback cb : shortcutCallbacks) { cb(shortcut); }
        }
    }
}

ShortcutManager *ShortcutManager::GetInstance()
{
    return EditorWindow::GetActive()->GetShortcutManager();
}


Shortcut::Shortcut(const Array<Key> &keys, const String &name)
{
    m_keys.PushBack(keys);
    m_name = name;
}
Shortcut::Shortcut(Key fk, const String &name)
                  : Shortcut( Array<Key>({fk}), name )
{  }
Shortcut::Shortcut(Key fk, Key sk, const String &name)
                  : Shortcut( Array<Key>({fk,sk}), name )
{  }
Shortcut::Shortcut(Key fk, Key sk, Key tk, const String &name)
                  : Shortcut( Array<Key>({fk,sk, tk}), name )
{  }

bool Shortcut::IsTriggered(const Array<Key> &keys) const
{
    {
        // for (Key k : GetKeys()) { if (!keys.Contains(k)) { return false; } }
    }

    {
        if (GetKeys().Size() != keys.Size()) { return false; }

        for (int i = 0; i < GetKeys().Size(); ++i)
        {
            if (GetKeys()[i] != keys[i]) { return false; }
        }
    }
    return true;
}

const Array<Key> &Shortcut::GetKeys() const
{
    return m_keys;
}

const String &Shortcut::GetName() const
{
    return m_name;
}

bool Shortcut::operator==(const Shortcut &rhs) const
{
    if (GetKeys().Size() != rhs.GetKeys().Size()) { return false; }

    for (int i = 0; i < GetKeys().Size(); ++i)
    {
        if (GetKeys()[i] != rhs.GetKeys()[i]) { return false; }
    }
    return true;
}

bool Shortcut::operator<(const Shortcut &rhs) const
{
    for (int i = 0; i < Math::Min(GetKeys().Size(), rhs.GetKeys().Size()); ++i)
    {
        if (GetKeys()[i] < rhs.GetKeys()[i]) { return true; }
        else if (GetKeys()[i] > rhs.GetKeys()[i]) { return false; }
    }
    return GetKeys().Size() < rhs.GetKeys().Size();
}
