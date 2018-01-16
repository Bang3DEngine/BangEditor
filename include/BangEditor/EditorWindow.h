#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "Bang/Window.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;
FORWARD class ShortcutManager;

class EditorWindow : public Window
{
public:
    EditorWindow();
    virtual ~EditorWindow();

    virtual void Update() override;
    virtual void Render() override;
    void OnClosed() override;

    void Create(uint flags) override;
    virtual SceneManager* CreateSceneManager() const override;

private:
    ShortcutManager *m_shortcutManager = nullptr;

    ShortcutManager *GetShortcutManager() const;
    static EditorWindow *GetActive();

    friend class ShortcutManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORWINDOW_H
