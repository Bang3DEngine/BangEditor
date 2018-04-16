#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "Bang/Application.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class DialogWindow;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Editor;
FORWARD class EditorScene;
FORWARD class EditorPaths;

class EditorApplication : public Application
{
public:
    EditorApplication();
    virtual ~EditorApplication();

    void InitEditorApplication(const Path &engineRootPath, 
                               const Path &editorRootPath);
    void OpenEditorScene();

    EditorPaths* GetEditorPaths() const;

private:
    Editor *m_editor = nullptr;

    Paths* CreatePaths() const override;
    Resources* CreateResources() const override;

    Editor *GetEditor() const;
    static EditorApplication* GetInstance();

    friend class Editor;
    friend class EditorPaths;
    friend class EditorBehaviourManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORAPPLICATION_H
