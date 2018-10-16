#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <sys/types.h>

#include "Bang/BangDefines.h"
#include "Bang/Window.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class SceneManager;
}  // namespace Bang

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;
FORWARD class ShortcutManager;

class EditorWindow : public Window
{
public:
    EditorWindow();
    virtual ~EditorWindow();

    void Create(uint flags) override;
    virtual void Update() override;
    virtual void Render() override;
    bool OnClosed() override;

private:
    ShortcutManager *m_shortcutManager = nullptr;

    SceneManager *CreateSceneManager() const override;

    ShortcutManager *GetShortcutManager() const;
    static EditorWindow *GetActive();

    friend class ShortcutManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORWINDOW_H
