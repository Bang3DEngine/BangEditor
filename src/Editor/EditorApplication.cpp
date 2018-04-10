#include "BangEditor/EditorApplication.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/SceneManager.h"
#include "Bang/ImportFilesManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorBehaviourManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication() : Application()
{
}

EditorApplication::~EditorApplication()
{
    delete m_editor;
}

void EditorApplication::InitEditorApplication(const Path &engineRootPath,
                                              const Path &editorRootPath)
{
    Application::Init(engineRootPath);

    GetEditorPaths()->InitEditorPath(editorRootPath);
    ImportFilesManager::CreateMissingImportFiles( EditorPaths::GetEditorAssetsDir() );
    ImportFilesManager::LoadImportFilepathGUIDs(  EditorPaths::GetEditorAssetsDir() );

    m_editor = new Editor();
}

EditorPaths *EditorApplication::GetEditorPaths() const
{
    return SCAST<EditorPaths*>(GetPaths());
}

void EditorApplication::OpenEditorScene()
{
    EditorScene *edScene = GameObject::Create<EditorScene>();
    EditorSceneManager::LoadSceneInstantly(edScene, false);
    edScene->Init();
    m_editor->Init();
}

EditorApplication *EditorApplication::GetInstance()
{
    return SCAST<EditorApplication*>( Application::GetInstance() );
}

Paths *EditorApplication::CreatePaths()
{
    return new EditorPaths();
}

Editor *EditorApplication::GetEditor() const
{
    return m_editor;
}
