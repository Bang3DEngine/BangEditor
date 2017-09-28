#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "BangEditor/BangEditor.h"

#include "Bang/Application.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class DialogWindow;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;

class EditorApplication : public Application
{
public:
    EditorApplication(int argc, char **argv,
                      const Path &engineRootPath = Path::Empty);
    virtual ~EditorApplication();

    SceneManager* CreateSceneManager() const override;

    void OpenEditorScene(Window *containingWindow);
    EditorScene *GetEditorScene() const;

private:
    Window* _CreateWindow() override;
    virtual void SetupWindow(Window *window) override;

    EditorScene *m_editorScene = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORAPPLICATION_H
