#ifndef EDITORAPPLICATION_H
#define EDITORAPPLICATION_H

#include "Bang/Application.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Debug;
class DialogWindow;
class Path;
class Paths;
class Resources;
class Scene;
class Texture2D;
class UIImageRenderer;
}

using namespace Bang;
namespace BangEditor
{
class Editor;
class EditorDebug;
class EditorPaths;
class EditorResources;
class EditorScene;

class EditorApplication : public Application
{
public:
    EditorApplication();
    virtual ~EditorApplication() override;

    void InitEditorApplication(const Path &engineRootPath,
                               const Path &editorRootPath);
    void OpenEditorScene();

    EditorPaths *GetEditorPaths() const;

private:
    Editor *m_editor = nullptr;

    Debug *CreateDebug() const override;
    Paths *CreatePaths() const override;
    Resources *CreateResources() const override;

    Editor *GetEditor() const;
    static EditorApplication *GetInstance();

    friend class Editor;
    friend class EditorPaths;
    friend class EditorBehaviourManager;
};
}

#endif  // EDITORAPPLICATION_H
