#include "BangEditor/Shortcut.h"

#include "Bang/Flags.h"
#include "Bang/Input.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Shortcut::Shortcut(Key key,
                   KeyModifiers keyModifiers,
                   const String &name,
                   bool autoRepeat)
{
    m_key = key;
    m_keyModifiers = keyModifiers;
    m_name = name;
    m_autoRepeat = autoRepeat;
}

bool Shortcut::IsTriggered(const InputEvent &inputEvent) const
{
    switch (inputEvent.type)
    {
        case InputEvent::Type::KEY_DOWN:
            if (GetAutoRepeat() || (!inputEvent.autoRepeat))
            {
                return (inputEvent.key == GetKey() &&
                        inputEvent.keyModifiers == GetKeyModifiers());
            }
        break;

        default:
        break;
    }
    return false;
}

bool Shortcut::GetAutoRepeat() const
{
    return m_autoRepeat;
}

Key Shortcut::GetKey() const
{
    return m_key;
}

KeyModifiers Shortcut::GetKeyModifiers() const
{
    return m_keyModifiers;
}

const String &Shortcut::GetName() const
{
    return m_name;
}

bool Shortcut::operator==(const Shortcut &rhs) const
{
    return GetKey() == rhs.GetKey() &&
           GetKeyModifiers() == rhs.GetKeyModifiers() &&
           GetName() == rhs.GetName();
}

bool Shortcut::operator<(const Shortcut &rhs) const
{
    if (GetKey() < rhs.GetKey())
    {
        return true;
    }
    else if (GetKey() == rhs.GetKey())
    {
        if (GetKeyModifiers().GetValue() < rhs.GetKeyModifiers().GetValue())
        {
            return true;
        }
        else if (GetKeyModifiers().GetValue() == rhs.GetKeyModifiers().GetValue())
        {
            return GetName() < rhs.GetName();
        }
        return false;
    }
    return false;
}
