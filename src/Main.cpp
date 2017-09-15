#include "BangEditor/EditorApplication.h"

#include "Bang/Scene.h"
#include "Bang/Project.h"
#include "Bang/ProjectManager.h"
#include "BangEditor/EditorScene.h"

#ifndef BANG_PROJECT_ROOT
    #define BANG_PROJECT_ROOT
#endif

using namespace Bang;
using namespace BangEditor;

int main(int argc, char **argv)
{
    EditorApplication edApp(argc, argv, Path("" BANG_PROJECT_ROOT));

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

    edApp.CreateWindow();

    return edApp.MainLoop();
}
