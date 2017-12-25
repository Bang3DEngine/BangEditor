#include "BangEditor/EditorApplication.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/SceneManager.h"
#include "Bang/ImportFilesManager.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication() : Application()
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::Init(const Path &engineRootPath)
{
    Application::Init(engineRootPath);

    GetEditorPaths()->InitEditorPath( Paths::ExecutablePath().GetDirectory()
                                                             .GetDirectory()
                                                             .GetDirectory());
    ImportFilesManager::AddAssetPath( EditorPaths::EditorResources() );
    ImportFilesManager::CreateMissingImportFiles();
}

EditorPaths *EditorApplication::GetEditorPaths() const
{
    return SCAST<EditorPaths*>(GetPaths());
}

void EditorApplication::OpenEditorScene()
{
    m_editorScene = GameObject::Create<EditorScene>();
    EditorSceneManager::LoadSceneInstantly(m_editorScene);
    m_editorScene->Init();
}

EditorApplication *EditorApplication::GetInstance()
{
    return SCAST<EditorApplication*>( Application::GetInstance() );
}

EditorScene *EditorApplication::GetEditorScene() const
{
    return m_editorScene;
}

Paths *EditorApplication::CreatePaths()
{
    return new EditorPaths();
}

Window *EditorApplication::_CreateWindow()
{
    return new EditorWindow();
}
