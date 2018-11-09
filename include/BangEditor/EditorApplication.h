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
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class Editor;
class EditorDebug;
class EditorPaths;
class EditorResources;
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
    Resources *CreateResources() const override;

    Editor *GetEditor() const;
    static EditorApplication *GetInstance();

    friend class Editor;
    friend class EditorPaths;
    friend class EditorBehaviourManager;
};
}  // namespace BangEditor

#endif  // EDITORAPPLICATION_H
