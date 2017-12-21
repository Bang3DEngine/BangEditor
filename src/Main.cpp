#include "BangEditor/EditorApplication.h"

#include "Bang/Scene.h"
#include "Bang/Window.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/ProjectManager.h"

#ifndef BANG_PROJECT_ROOT
    #define BANG_PROJECT_ROOT
#endif

using namespace Bang;
using namespace BangEditor;

int main(int argc, char **argv)
{
    EditorApplication edApp;
    edApp.Init( Path("" BANG_PROJECT_ROOT) );

    Path projectToOpenPath;
    if (argc >= 2)
    {
        projectToOpenPath = Path(argv[1]);
        if (projectToOpenPath.IsFile())
        {
            ProjectManager::OpenProject(projectToOpenPath);
        }
        else
        {
            Debug_Error("Can't find project '" << projectToOpenPath << "'.");
        }
    }

    Window *mainWindow = edApp.CreateWindow();
    mainWindow->SetTitle("Bang Editor");
    edApp.OpenEditorScene(mainWindow);

    return edApp.MainLoop();
}
