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
class Assets;
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
class EditorAssets;
class EditorScene;
class EditorSettings;

class EditorApplication : public Application
{
public:
    EditorApplication();
    virtual ~EditorApplication() override;

    void Init(const Path &engineRootPath, const Path &editorRootPath);
    void OpenEditorScene();

    EditorPaths *GetEditorPaths() const;
    EditorSettings *GetEditorSettings() const;

private:
    Editor *m_editor = nullptr;

    Debug *CreateDebug() const override;
    Paths *CreatePaths() const override;
    Settings *CreateSettings() const override;
    Assets *CreateAssets() const override;
    ProjectManager *CreateProjectManager() const override;

    Editor *GetEditor() const;
    static EditorApplication *GetInstance();

    friend class Editor;
    friend class EditorPaths;
    friend class EditorBehaviourManager;
};
}  // namespace BangEditor

#endif  // EDITORAPPLICATION_H
