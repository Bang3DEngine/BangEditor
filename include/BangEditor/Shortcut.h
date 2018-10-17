#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
struct InputEvent;
}

using namespace Bang;
namespace BangEditor
{
class Shortcut
{
public:
    Shortcut(Key key,
             KeyModifiers keyModifiers,
             const String &name = "",
             bool autoRepeat = false);

    bool IsTriggered(const InputEvent &inputEvent) const;

    Key GetKey() const;
    KeyModifiers GetKeyModifiers() const;
    const String &GetName() const;
    bool GetAutoRepeat() const;

    bool operator==(const Shortcut &rhs) const;
    bool operator<(const Shortcut &rhs) const;

private:
    Key m_key;
    KeyModifiers m_keyModifiers;

    String m_name;
    bool m_autoRepeat;
};
}

#endif  // SHORTCUT_H
