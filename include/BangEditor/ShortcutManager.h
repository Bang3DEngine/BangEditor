#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include "Bang/Set.h"
#include "Bang/List.h"
#include "Bang/Input.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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




class ShortcutManager
{
public:
    using ShortcutCallback = std::function<void(const Shortcut &shortcut)>;

    static void RegisterShortcut(const Shortcut &shortcut,
                                 ShortcutCallback callback);

private:
    Set<Shortcut> m_shortcutsTriggeredWithCurrentKeys;
    Map<Shortcut, Array<ShortcutCallback>> m_shortcuts;
    Array<Key> m_pressedKeys;

    ShortcutManager();
    virtual ~ShortcutManager();

    void Update();

    static ShortcutManager *GetInstance();

    friend class EditorWindow;
};

NAMESPACE_BANG_EDITOR_END

#endif // SHORTCUTMANAGER_H

