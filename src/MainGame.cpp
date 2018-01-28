#include "Bang/Application.h"

#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Scene.h"
#include "Bang/Window.h"
#include "Bang/Library.h"
#include "Bang/Extensions.h"
#include "Bang/SceneManager.h"
#include "Bang/BehaviourManager.h"
#include "Bang/BehaviourContainer.h"
#include "Bang/ImportFilesManager.h"

USING_NAMESPACE_BANG

int main(int, char **)
{
    // Get path and directories, and check for their existence.
    Path execPath     = Paths::GetExecutablePath();
    Path execDir      = execPath.GetDirectory();
    Path dataDir      = execDir.Append("Data");
    if (!dataDir.IsDir())
    { Debug_Error("Could not find data directory '" << dataDir << "'."); return 1; }

    Path resDir = dataDir.Append("res");
    if (!resDir.IsDir())
    { Debug_Error("Could not find resources directory '" << resDir << "'."); return 2; }

    Path engineAssetsDir = resDir.Append("EngineAssets");
    if (!engineAssetsDir.IsDir())
    { Debug_Error("Could not find engine assets directory '" << engineAssetsDir << "'."); return 3; }

    Path gameAssetsDir = resDir.Append("Assets");
    if (!gameAssetsDir.IsDir())
    { Debug_Error("Could not find game assets directory '" << gameAssetsDir << "'."); return 4; }

    Path librariesDir = dataDir.Append("Libraries");
    if (!librariesDir.IsDir())
    { Debug_Error("Could not find libraries directory '" << librariesDir << "'."); return 5; }

    Application app;
    app.Init(dataDir);
    ImportFilesManager::CreateMissingImportFiles(gameAssetsDir);
    ImportFilesManager::LoadImportFilepathGUIDs(gameAssetsDir);

    Window *mainWindow = app.CreateWindow();
    Window::SetActive(mainWindow);
    mainWindow->SetTitle("Bang");

    List<Path> sceneFilepaths = gameAssetsDir.GetFiles(Path::FindFlag::Recursive,
                                                {Extensions::GetSceneExtension()});
    if (sceneFilepaths.IsEmpty())
    { Debug_Error("No scene found in '" << gameAssetsDir << "'");
        return 6;
    }

    // Load the first scene
    Path scenePath = sceneFilepaths.Front();
    Debug_Log("Opening scene " << scenePath);
    SceneManager::LoadSceneInstantly(scenePath);

    // Find the behaviours library
    Path behavioursLibPath;
    List<Path> libPaths = librariesDir.GetFiles(Path::FindFlag::Simple);
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
        Debug_Error("No behaviours library found in '" << librariesDir << "'");
        return 7;
    }

    Debug_Log("Picking as Behaviours library: '" <<
              behavioursLibPath.GetAbsolute() << "'");

    // Set the behaviour library
    SceneManager::GetInstance()->GetBehaviourManager()->
                  SetBehavioursLibrary(behavioursLibPath);

    return app.MainLoop();
}
