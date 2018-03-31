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

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

int main(int argc, char **argv)
{
    EditorApplication edApp;
    const Path engPath = Paths::GetResolvedPath( Path("" BANG_ENGINE_ROOT) );
    const Path edtPath = Paths::GetResolvedPath( Path("" BANG_EDITOR_ROOT) );
    Debug_Log("BangEngineRoot: " << "" BANG_ENGINE_ROOT << " => " << engPath);
    Debug_Log("BangEditorRoot: " << "" BANG_EDITOR_ROOT << " => " << edtPath);
    edApp.InitEditorApplication(engPath, edtPath);

    Window *mainWindow = WindowManager::CreateWindow<EditorWindow>();
    Window::SetActive(mainWindow);
    mainWindow->SetTitle("Bang Editor");
    edApp.OpenEditorScene();

    // Open project (arg or latest one)
    Path projectToOpenPath = (argc >= 2) ? 
          Path(argv[1]) : EditorSettings::GetLatestProjectFilepathOpen();

    if (projectToOpenPath.IsFile())
    {
        ProjectManager::OpenProject(projectToOpenPath);
    }
    else  
    {
        if (argc >= 2) 
        { 
            Debug_Error("Can't find project '" << projectToOpenPath << "'."); 
        }
        SceneOpenerSaver::GetInstance()->OpenDefaultScene();
    }


    return edApp.MainLoop();
}
