#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "Bang/Bang.h"
#include "Bang/Array.h"
#include "Bang/Key.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG

NAMESPACE_BANG_EDITOR_BEGIN

class Shortcut
{
public:
    Shortcut(Key firstKey, const String &name = "",
             bool autoRepeat = false);
    Shortcut(Key firstKey, Key secondKey, const String &name = "",
             bool autoRepeat = false);
    Shortcut(Key firstKey, Key secondKey, Key thirdKey, const String &name = "",
             bool autoRepeat = false);

    Shortcut(const Array<Key> &keys,
             const String &name = "",
             bool autoRepeat = false);

    bool IsTriggered(const Array<Key> &pressedKeysNow,
                     const Array<Key> &downKeysNow,
                     const Array<Key> &downRepeatKeysNow) const;
    bool IsAutoRepeat() const;

    const Array<Key>& GetKeys() const;
    const String& GetName() const;

    bool operator==(const Shortcut &rhs) const;
    bool operator<(const Shortcut &rhs) const;

private:
    String m_name;
    Array<Key> m_keys;
    bool m_autoRepeat;
};

NAMESPACE_BANG_EDITOR_END

// Shortcut Hash
namespace std
{
    template <>
    struct hash<BangEditor::Shortcut>
    {
        std::size_t operator()(const BangEditor::Shortcut& shortcut) const
        {
            std::size_t hashInt = 0;
            for (const Key &k : shortcut.GetKeys())
            {
                hashInt = hashInt ^ SCAST<int>(k);
            }
            return hashInt;
        }
    };
}

#endif // SHORTCUT_H

