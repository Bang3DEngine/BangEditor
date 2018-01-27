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
#include "BangEditor/EditorBehaviourManager.h"
#include "BangEditor/EditorComponentFactory.h"

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

    GetEditorPaths()->InitEditorPath( Paths::GetExecutablePath().GetDirectory()
                                                                .GetDirectory()
                                                                .GetDirectory());
    ImportFilesManager::CreateMissingImportFiles( EditorPaths::GetEditorResourcesDir() );
    ImportFilesManager::LoadImportFilepathGUIDs( EditorPaths::GetEditorResourcesDir() );
}

EditorPaths *EditorApplication::GetEditorPaths() const
{
    return SCAST<EditorPaths*>(GetPaths());
}

void EditorApplication::OpenEditorScene()
{
    // Window::GetActive()->GetGlobalObjectManager()->StartObjects();
    EditorScene *edScene = GameObject::Create<EditorScene>();
    // Window::GetActive()->GetGlobalObjectManager()->StartObjects();
    EditorSceneManager::LoadSceneInstantly(edScene);
    edScene->Init();
    // Window::GetActive()->GetGlobalObjectManager()->StartObjects();
}

EditorApplication *EditorApplication::GetInstance()
{
    return SCAST<EditorApplication*>( Application::GetInstance() );
}

Paths *EditorApplication::CreatePaths()
{
    return new EditorPaths();
}

Window *EditorApplication::_CreateWindow()
{
    return new EditorWindow();
}

ComponentFactory *EditorApplication::CreateComponentFactory()
{
    return new EditorComponentFactory();
}
