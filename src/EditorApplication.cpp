#include "BangEditor/EditorApplication.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Project.h"
#include "Bang/SceneManager.h"
#include "Bang/ProjectManager.h"
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

    Scene *scene = new Scene();
    Project *proj = ProjectManager::GetCurrentProject();
    if (proj)
    {
        Debug_Log(proj->GetFirstFoundScenePath());
        scene->ImportXMLFromFile(proj->GetFirstFoundScenePath());
    }
    m_editorScene->SetOpenScene(scene);
}

EditorScene *EditorApplication::GetEditorScene() const
{
    return m_editorScene;
}
