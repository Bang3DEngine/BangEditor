#include "BangEditor/EditorApplication.h"

#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/MetaFilesManager.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorDebug.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorResources.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorSettings.h"

namespace Bang
{
class Debug;
class Path;
class Paths;
class Resources;
}  // namespace Bang

using namespace Bang;
using namespace BangEditor;

EditorApplication::EditorApplication() : Application()
{
}

EditorApplication::~EditorApplication()
{
    delete m_editor;
}

void EditorApplication::Init(const Path &engineRootPath,
                             const Path &editorRootPath)
{
    Application::Init_(engineRootPath);
    GetEditorPaths()->InitEditorPath(editorRootPath);
    InitAfterPathsInit_();
    GetEditorSettings()->ExportToFile();
    EditorResources::GetInstance()->InitAfterGLIsInited();

    MetaFilesManager::CreateMissingMetaFiles(EditorPaths::GetEditorAssetsDir());
    MetaFilesManager::LoadMetaFilepathGUIDs(EditorPaths::GetEditorAssetsDir());

    m_editor = new Editor();
}

EditorPaths *EditorApplication::GetEditorPaths() const
{
    return SCAST<EditorPaths *>(GetPaths());
}

EditorSettings *BangEditor::EditorApplication::GetEditorSettings() const
{
    return SCAST<EditorSettings *>(GetSettings());
}

void EditorApplication::OpenEditorScene()
{
    EditorScene *edScene = new EditorScene();
    EditorSceneManager::LoadSceneInstantly(edScene, false);
    edScene->Init();
    m_editor->Init();
}

EditorApplication *EditorApplication::GetInstance()
{
    return SCAST<EditorApplication *>(Application::GetInstance());
}

Debug *EditorApplication::CreateDebug() const
{
    return new EditorDebug();
}

Paths *EditorApplication::CreatePaths() const
{
    return new EditorPaths();
}

Settings *EditorApplication::CreateSettings() const
{
    return new EditorSettings();
}

Resources *EditorApplication::CreateResources() const
{
    return new EditorResources();
}

Editor *EditorApplication::GetEditor() const
{
    return m_editor;
}
