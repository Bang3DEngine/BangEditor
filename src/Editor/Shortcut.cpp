#include "BangEditor/Shortcut.h"

#include "Bang/Math.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Shortcut::Shortcut(const Array<Key> &keys, const String &name, bool autoRepeat)
{
    m_keys.PushBack(keys);
    m_name = name;
    m_autoRepeat = autoRepeat;
}
Shortcut::Shortcut(Key fk, const String &name, bool autoRepeat)
                  : Shortcut( Array<Key>({fk}), name, autoRepeat)
{
}

Shortcut::Shortcut(Key fk, Key sk, const String &name, bool autoRepeat)
                  : Shortcut( Array<Key>({fk,sk}), name, autoRepeat)
{
}

Shortcut::Shortcut(Key fk, Key sk, Key tk, const String &name, bool autoRepeat)
                  : Shortcut( Array<Key>({fk,sk, tk}), name, autoRepeat)
{
}

bool Shortcut::IsTriggered(const Array<Key> &pressedKeysNow,
                           const Array<Key> &downKeysNow,
                           const Array<Key> &downRepeatKeysNow) const
{
    if (GetKeys().Size() == 0) { return false; }
    if (GetKeys().Size() != pressedKeysNow.Size()) { return false; }

    bool allKeysPressed = true;
    bool atLeastOneKeyDownNow = false;
    for (Key k : GetKeys())
    {
        atLeastOneKeyDownNow = atLeastOneKeyDownNow ||
                              (downKeysNow.Contains(k) ||
                               (IsAutoRepeat() && downRepeatKeysNow.Contains(k))
                              );

        if (!pressedKeysNow.Contains(k))
        {
            allKeysPressed = false;
            break;
        }
    }

    return allKeysPressed && atLeastOneKeyDownNow;
}

bool Shortcut::IsAutoRepeat() const
{
    return m_autoRepeat;
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
