#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
struct InputEvent;
}  // namespace Bang

USING_NAMESPACE_BANG

NAMESPACE_BANG_EDITOR_BEGIN

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
    const String& GetName() const;
    bool GetAutoRepeat() const;

    bool operator==(const Shortcut &rhs) const;
    bool operator<(const Shortcut &rhs) const;

private:
    Key m_key;
    KeyModifiers m_keyModifiers;

    String m_name;
    bool m_autoRepeat;
};

NAMESPACE_BANG_EDITOR_END

#endif // SHORTCUT_H

