#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "Bang/Window.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;

class EditorWindow : public Window
{
public:
    EditorWindow();
    virtual ~EditorWindow();

    virtual void Update() override;
    virtual void Render() override;

    virtual SceneManager* CreateSceneManager() const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORWINDOW_H
