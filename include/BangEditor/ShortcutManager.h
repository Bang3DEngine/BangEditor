#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <unordered_set>

#include "Bang/List.h"
#include "Bang/UMap.h"
#include "Bang/USet.h"
#include "Bang/Input.h"
#include "Bang/String.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/Shortcut.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ShortcutManager
{
public:
    using ShortcutCallback = std::function<void(const Shortcut &shortcut)>;

    static void RegisterShortcut(const Shortcut &shortcut,
                                 ShortcutCallback callback);

private:
    UMap<Shortcut, Array<ShortcutCallback>> m_shortcuts;

    ShortcutManager();
    virtual ~ShortcutManager();

    void Update();

    static ShortcutManager *GetInstance();

    friend class EditorWindow;
};

NAMESPACE_BANG_EDITOR_END

#endif // SHORTCUTMANAGER_H

