#include "BangEditor/EditorApplication.h"

#include "Bang/Path.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/WindowManager.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/SceneOpenerSaver.h"
#include "BangEditor/SelectProjectWindow.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

int main(int argc, char **argv)
{
    EditorApplication editorApplication;
    const Path engPath = Paths::GetResolvedPath( Path("" BANG_ENGINE_ROOT) );
    const Path edtPath = Paths::GetResolvedPath( Path("" BANG_EDITOR_ROOT) );
    Debug_Log("BangEngineRoot: " << "" BANG_ENGINE_ROOT << " => " << engPath);
    Debug_Log("BangEditorRoot: " << "" BANG_EDITOR_ROOT << " => " << edtPath);
    editorApplication.InitEditorApplication(engPath, edtPath);

    Path projectToBeOpen = Path::Empty;
    if (argc < 2)
    {
        // Select project window
        SelectProjectWindow *selectProjectWindow =
                             WindowManager::CreateWindow<SelectProjectWindow>();
        Window::SetActive(selectProjectWindow);
        selectProjectWindow->Init();
        editorApplication.MainLoop();

        projectToBeOpen = SelectProjectWindow::OpenProjectResult;
    }
    else { projectToBeOpen = Path(argv[1]); }

    if (projectToBeOpen.IsEmpty()) { Application::Exit(0, true); }

    if (!projectToBeOpen.IsFile())
    {
        Debug_Error("Could not open project '" << projectToBeOpen << "'");
        Application::Exit(1, true);
    }


    // Main window
    Window *mainWindow = WindowManager::CreateWindow<EditorWindow>();
    Window::SetActive(mainWindow);
    editorApplication.OpenEditorScene();

    ProjectManager::OpenProject(projectToBeOpen);

    return editorApplication.MainLoop();
}
