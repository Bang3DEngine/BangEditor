#include "Bang/Application.h"

#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Library.h"
#include "Bang/Extensions.h"
#include "Bang/SceneManager.h"
#include "Bang/WindowManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/BehaviourContainer.h"

USING_NAMESPACE_BANG

int main(int argc, char **argv)
{
    Path execDir = (argc > 1) ? Path(argv[1]) : Paths::GetExecutableDir();

    // Get path and directories, and check for their existence.
    Path dataDir      = execDir.Append("Data");
    if (!dataDir.IsDir())
    {
        Debug_Error("Could not find data directory '" << dataDir << "'.");
        return 1;
    }

    Path bangDataDir = dataDir.Append("Bang");
    if (!bangDataDir.IsDir())
    {
        Debug_Error("Could not find bang data directory '" << bangDataDir << "'.");
        return 3;
    }

    Path gameAssetsDir = dataDir.Append("Assets");
    if (!gameAssetsDir.IsDir())
    {
        Debug_Error("Could not find game assets directory '" << gameAssetsDir << "'.");
        return 4;
    }

    Path librariesDir = dataDir.Append("Libraries");
    if (!librariesDir.IsDir())
    {
        Debug_Error("Could not find libraries directory '" << librariesDir << "'.");
        return 5;
    }

    Application app;
    app.Init(bangDataDir);

    MetaFilesManager::CreateMissingMetaFiles(gameAssetsDir);
    MetaFilesManager::LoadMetaFilepathGUIDs(gameAssetsDir);

    Paths::SetProjectRoot(dataDir);

    Window *mainWindow = WindowManager::CreateWindow<Window>();
    Window::SetActive(mainWindow);
    mainWindow->SetTitle("Bang");
    mainWindow->Maximize();

    Array<Path> sceneFilepaths = gameAssetsDir.GetFiles(FindFlag::RECURSIVE,
                                                {Extensions::GetSceneExtension()});
    if (sceneFilepaths.IsEmpty())
    {
        Debug_Error("No scene found in '" << gameAssetsDir << "'");
        return 6;
    }

    // Load the first scene
    Paths::SortPathsByName(&sceneFilepaths);
    Path scenePath = sceneFilepaths.Back();
    Debug_Log("Opening scene " << scenePath);
    SceneManager::LoadSceneInstantly(scenePath, false);

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
        // return 7;
    }

    Debug_Log("Picking as Behaviours library: '" <<
              behavioursLibPath.GetAbsolute() << "'");

    // Set the behaviour library
    SceneManager::GetActive()->GetBehaviourManager()->
                  SetBehavioursLibrary(behavioursLibPath);

    return app.MainLoop();
}
