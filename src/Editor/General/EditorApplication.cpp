#include "BangEditor/EditorApplication.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/SceneManager.h"
#include "Bang/MetaFilesManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Project.h"
#include "BangEditor/EditorDebug.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorResources.h"
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
    EditorResources::GetInstance()->InitAfterGLIsInited();

    GetEditorPaths()->InitEditorPath(editorRootPath);
    MetaFilesManager::CreateMissingMetaFiles( EditorPaths::GetEditorAssetsDir() );
    MetaFilesManager::LoadMetaFilepathGUIDs(  EditorPaths::GetEditorAssetsDir() );

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


Debug *EditorApplication::CreateDebug() const
{
    return new EditorDebug();
}

Paths *EditorApplication::CreatePaths() const
{
    return new EditorPaths();
}

Resources *EditorApplication::CreateResources() const
{
    return new EditorResources();
}

Editor *EditorApplication::GetEditor() const
{
    return m_editor;
}
