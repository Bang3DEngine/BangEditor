#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <unordered_set>

#include "Bang/List.h"
#include "Bang/UMap.h"
#include "Bang/USet.h"
#include "Bang/Input.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

// Shortcut
class Shortcut
{
public:
    Shortcut(Key firstKey, const String &name = "");
    Shortcut(Key firstKey, Key secondKey, const String &name = "");
    Shortcut(Key firstKey, Key secondKey, Key thirdKey, const String &name = "");
    Shortcut(const Array<Key> &keys, const String &name = "");

    bool IsTriggered(const Array<Key> &keys) const;

    const Array<Key>& GetKeys() const;
    const String& GetName() const;

    bool operator==(const Shortcut &rhs) const;
    bool operator<(const Shortcut &rhs) const;

private:
    String m_name;
    Array<Key> m_keys;
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

NAMESPACE_BANG_EDITOR_BEGIN

// ShortcutManager
class ShortcutManager
{
public:
    using ShortcutCallback = std::function<void(const Shortcut &shortcut)>;

    static void RegisterShortcut(const Shortcut &shortcut,
                                 ShortcutCallback callback);

private:
    USet<Shortcut> m_shortcutsTriggeredWithCurrentKeys;
    UMap<Shortcut, Array<ShortcutCallback>> m_shortcuts;
    Array<Key> m_pressedKeys;

    ShortcutManager();
    virtual ~ShortcutManager();

    void Update();

    static ShortcutManager *GetInstance();

    friend class EditorWindow;
};

NAMESPACE_BANG_EDITOR_END

#endif // SHORTCUTMANAGER_H

