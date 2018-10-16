#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "Bang/Application.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class Debug;
class Path;
class Paths;
class Resources;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class DialogWindow;
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD class UIImageRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Editor;
FORWARD class EditorDebug;
FORWARD class EditorPaths;
FORWARD class EditorResources;
FORWARD class EditorScene;

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

    Debug* CreateDebug() const override;
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
