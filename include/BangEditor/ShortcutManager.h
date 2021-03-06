#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <functional>
#include <unordered_set>

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/Input.h"
#include "Bang/List.h"
#include "Bang/Map.h"
#include "Bang/String.h"
#include "Bang/USet.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Shortcut.h"

using namespace Bang;
namespace BangEditor
{
class ShortcutManager
{
public:
    using ShortcutCallback = std::function<void(const Shortcut &shortcut)>;

    static void RegisterShortcut(const Shortcut &shortcut,
                                 ShortcutCallback callback);

private:
    Map<Shortcut, Array<ShortcutCallback>> m_shortcuts;

    ShortcutManager();
    virtual ~ShortcutManager();

    void Update();

    static ShortcutManager *GetInstance();

    friend class EditorWindow;
};
}

#endif  // SHORTCUTMANAGER_H
