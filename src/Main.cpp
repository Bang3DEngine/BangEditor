#include "BangEditor/EditorApplication.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/WindowManager.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorWindow.h"
#include "BangEditor/EditorSettings.h"
#include "BangEditor/ProjectManager.h"

#ifndef BANG_PROJECT_ROOT
    #define BANG_PROJECT_ROOT
#endif

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

int main(int argc, char **argv)
{
    EditorApplication edApp;
    edApp.Init( Path("" BANG_PROJECT_ROOT) );

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
