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
#include "BangEditor/BehaviourManager.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorComponentFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication() : Application()
{
}

EditorApplication::~EditorApplication()
{
    delete m_behaviourManager;
}

void EditorApplication::Init(const Path &engineRootPath)
{
    Application::Init(engineRootPath);

    GetEditorPaths()->InitEditorPath( Paths::ExecutablePath().GetDirectory()
                                                             .GetDirectory()
                                                             .GetDirectory());
    ImportFilesManager::CreateMissingImportFiles( EditorPaths::EditorResources() );
    ImportFilesManager::LoadImportFilepathGUIDs( EditorPaths::EditorResources() );

    m_behaviourManager = new BehaviourManager();
}

EditorPaths *EditorApplication::GetEditorPaths() const
{
    return SCAST<EditorPaths*>(GetPaths());
}

void EditorApplication::OpenEditorScene()
{
    EditorScene *edScene = GameObject::Create<EditorScene>();
    EditorSceneManager::LoadSceneInstantly(edScene);
    edScene->Init();
}

EditorApplication *EditorApplication::GetInstance()
{
    return SCAST<EditorApplication*>( Application::GetInstance() );
}

BehaviourManager *EditorApplication::GetBehaviourManager() const
{
    return m_behaviourManager;
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
