#include "BangEditor/EditorApplication.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/SceneManager.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication(int argc, char **argv,
                                     const Path &engineRootPath)
    : Application(argc, argv, engineRootPath)
{
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::CreateWindow()
{
    Application::CreateWindow();

    m_editorScene = new EditorScene();
    SceneManager::LoadScene(m_editorScene);
}

void EditorApplication::RenderScene()
{
    m_editorScene->RenderEditor();
}

EditorScene *EditorApplication::GetEditorScene() const
{
    return m_editorScene;
}
