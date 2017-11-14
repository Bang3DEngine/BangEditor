#include "BangEditor/EditorApplication.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Project.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorApplication::EditorApplication(int argc, char **argv,
                                     const Path &engineRootPath)
    : Application(argc, argv, engineRootPath)
{
    GetPaths()->InitEditorPath( Paths::ExecutablePath().GetDirectory()
                                                       .GetDirectory()
                                                       .GetDirectory());
}

EditorApplication::~EditorApplication()
{
}

void EditorApplication::OpenEditorScene(Window *containingWindow)
{
    m_editorScene = ObjectManager::Create<EditorScene>();
    containingWindow->GetSceneManager()->LoadScene(m_editorScene);
}

void EditorApplication::SetupWindow(Window *window)
{
    Application::SetupWindow(window);
}

EditorScene *EditorApplication::GetEditorScene() const
{
    return m_editorScene;
}

Window *EditorApplication::_CreateWindow()
{
    return new EditorWindow();
}
