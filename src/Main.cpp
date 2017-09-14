#include "BangEditor/EditorApplication.h"

#include "Bang/Scene.h"
#include "BangEditor/EditorScene.h"

#ifndef BANG_PROJECT_ROOT
    #define BANG_PROJECT_ROOT
#endif

using namespace Bang;
using namespace BangEditor;

int main(int argc, char **argv)
{
    EditorApplication edApp(argc, argv, Path("" BANG_PROJECT_ROOT));
    edApp.CreateWindow();

    Path sceneToOpenPath;
    if (argc >= 2)
    {
        sceneToOpenPath = Path(argv[1]);
        if (!sceneToOpenPath.IsFile())
        {
            Debug_Error("Can't find scene '" << sceneToOpenPath << "'.");
        }
    }

    if (sceneToOpenPath.IsFile())
    {
        Scene *scene = new Scene();
        scene->ImportXMLFromFile(sceneToOpenPath);
        edApp.GetEditorScene()->SetOpenScene(scene);
    }

    return edApp.MainLoop();
}
