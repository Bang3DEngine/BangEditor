#include <ostream>

#include "Bang/Application.h"
#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/BehaviourManager.h"
#include "Bang/Debug.h"
#include "Bang/Extensions.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/Path.h"
#include "Bang/Paths.h"
#include "Bang/ProjectManager.h"
#include "Bang/SceneManager.h"
#include "Bang/StreamOperators.h"
#include "Bang/String.h"
#include "Bang/Window.h"
#include "Bang/WindowManager.h"
#include "Bang/WindowManager.tcc"

using namespace Bang;

int main(int argc, char **argv)
{
    Path execDir = (argc > 1) ? Path(argv[1]) : Paths::GetExecutableDir();

    // Get path and directories, and check for their existence.
    Path dataDir = execDir.Append("Data");
    if (!dataDir.IsDir())
    {
        Debug_Error("Could not find data directory '" << dataDir << "'.");
        return 1;
    }

    Path bangDataDir = dataDir.Append("Bang");
    if (!bangDataDir.IsDir())
    {
        Debug_Error("Could not find bang data directory '" << bangDataDir
                                                           << "'.");
        return 3;
    }

    Path gameAssetsDir = dataDir.Append("Assets");
    if (!gameAssetsDir.IsDir())
    {
        Debug_Error("Could not find game assets directory '" << gameAssetsDir
                                                             << "'.");
        return 4;
    }

    Path librariesDir = dataDir.Append("Libraries");
    if (!librariesDir.IsDir())
    {
        Debug_Error("Could not find libraries directory '" << librariesDir
                                                           << "'.");
        return 5;
    }

    Array<Path> projectFilepaths = dataDir.GetFiles(
        FindFlag::DEFAULT, {Extensions::GetProjectExtension()});
    Path projectFilepath =
        (projectFilepaths.Size() >= 1 ? projectFilepaths.Front()
                                      : Path::Empty());
    if (projectFilepath.IsEmpty())
    {
        Debug_Error("Could not find project filepath in '" << dataDir
                                                           << "' directory.");
        return 6;
    }

    Application app;
    app.Init(bangDataDir);

    Window *mainWindow = WindowManager::CreateWindow<Window>();
    Window::SetActive(mainWindow);
    mainWindow->SetTitle("Bang");
    mainWindow->Maximize();

    if (!ProjectManager::GetInstance()->OpenProject(projectFilepath))
    {
        Debug_Error("Could not open project or no scene was found...");
        return 7;
    }

    // Find the behaviours library
    Path behavioursLibPath;
    Array<Path> libPaths = librariesDir.GetFiles(FindFlag::SIMPLE);
    for (const Path &libPath : libPaths)
    {
        if (libPath.GetExtension().Contains("so"))
        {
            behavioursLibPath = libPath;
            break;
        }
    }

    if (!behavioursLibPath.IsFile())
    {
        Debug_Warn("No behaviours library found in '" << librariesDir << "'");
        // return 8;
    }
    else
    {
        Debug_Log("Picking as Behaviours library: '"
                  << behavioursLibPath.GetAbsolute()
                  << "'");
    }

    // Set the behaviour library
    SceneManager::GetActive()->GetBehaviourManager()->SetBehavioursLibrary(
        behavioursLibPath);

    return app.MainLoop();
}
