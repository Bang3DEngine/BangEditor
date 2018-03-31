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

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

int main(int argc, char **argv)
{
    EditorApplication edApp;
    const Path engineRootPath = 
                     Paths::GetResolvedPath( Path("" BANG_ENGINE_ROOT) );
    const Path editorRootPath = 
                     Paths::GetResolvedPath( Path("" BANG_EDITOR_ROOT) );
    Debug_Log("BangEngineRoot: " << "" BANG_ENGINE_ROOT << " => " << engineRootPath);
    Debug_Log("BangEditorRoot: " << "" BANG_EDITOR_ROOT << " => " << editorRootPath);
    edApp.InitEditorApplication(engineRootPath, editorRootPath);

    Window *mainWindow = WindowManager::CreateWindow<EditorWindow>();
    Window::SetActive(mainWindow);
    mainWindow->SetTitle("Bang Editor");
    edApp.OpenEditorScene();

    // Open project (arg or latest one)
    Path projectToOpenPath = EditorSettings::GetLatestProjectFilepathOpen();
    if (argc >= 2)
    {
        projectToOpenPath = Path(argv[1]);
        if (!projectToOpenPath.IsFile())
        {
            Debug_Error("Can't find project '" << projectToOpenPath << "'.");
        }
    }

    if (projectToOpenPath.IsFile())
    {
        ProjectManager::OpenProject(projectToOpenPath);
    }

    return edApp.MainLoop();
}
