#include "BangEditor/GameBuilder.h"

#include <ostream>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangPreprocessor.h"
#include "Bang/Compiler.h"
#include "Bang/Debug.h"
#include "Bang/Extensions.h"
#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/StreamOperators.h"
#include "Bang/Time.h"
#include "BangEditor/EditorBehaviourManager.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorProject.h"
#include "BangEditor/EditorProjectManager.h"

using namespace Bang;
using namespace BangEditor;

Path GameBuilder::GetExecutablePath()
{
    EditorProject *project =
        EditorProjectManager::GetInstance()->GetCurrentProject();
    Path outputExecutableFilepath =
        Paths::GetProjectDir()
            .Append(project->GetProjectName() + "_Game")
            .Append(project->GetProjectName())
            .AppendExtension("exe");
    return outputExecutableFilepath;
}

void GameBuilder::BuildGame()
{
    Path gameExecPath = GameBuilder::GetExecutablePath();
    File::CreateDir(gameExecPath.GetDirectory());

    GameBuilder::BuildGame(gameExecPath.GetName(),
                           gameExecPath.GetDirectory(),
                           // BinType::BIN_DEBUG,
                           BinType::BIN_RELEASE,
                           true);
}

void GameBuilder::BuildGame(const String &gameName,
                            const Path &gameDir,
                            BinType binaryType,
                            bool compileBehaviours)
{
    Array<Path> sceneFiles = Paths::GetProjectAssetsDir().GetFiles(
        FindFlag::RECURSIVE, {Extensions::GetSceneExtension()});
    if (sceneFiles.IsEmpty())
    {
        Debug_Error("Please save at least one scene in the "
                    "Assets directory to build the game");
        return;
    }

#define BANG_BUILD_GAME_CHECK_EXISTANCE(path)       \
    if (!path.Exists())                             \
    {                                               \
        Debug_Error("'" << path << "' not found."); \
        return;                                     \
    }

    const Path binariesDir = EditorPaths::GetEditorBinariesDir();
    const Path librariesDir = EditorPaths::GetEditorLibrariesDir();
    const Path gameBinaryTemplatePath = binariesDir.Append("GameTemplate");
    const Path gameBinaryPath = gameDir.Append(gameName).AppendExtension("exe");
    BANG_BUILD_GAME_CHECK_EXISTANCE(binariesDir);
    BANG_BUILD_GAME_CHECK_EXISTANCE(librariesDir);
    BANG_BUILD_GAME_CHECK_EXISTANCE(gameBinaryTemplatePath);

    Debug_Log("Copying assets into data directory...");
    if (!GameBuilder::CreateDataDirectory(gameDir))
    {
        Debug_Error("Could not create data directory");
        return;
    }

    if (compileBehaviours)
    {
        Debug_Log("Compiling behaviours...");
        if (!GameBuilder::CreateBehavioursLibrary(gameDir, binaryType))
        {
            Debug_Error("Could not compile behaviours!");
            return;
        }
    }

    Debug_Log("Moving the Game executable to '" << gameBinaryPath << "'...");
    File::Duplicate(gameBinaryTemplatePath, gameBinaryPath);
    File::AddExecutablePermission(gameBinaryPath);

    Debug_Log("Build finished successfully! "
              "Game path: '"
              << gameBinaryPath
              << "'");
}

bool GameBuilder::CreateDataDirectory(const Path &executableDir)
{
    Path dataDir = executableDir.Append("Data");
    File::Remove(dataDir);
    if (!File::CreateDir(dataDir))
    {
        return false;
    }

    // Copy project file
    Path projectFilepath = EditorProjectManager::GetInstance()
                               ->GetCurrentProject()
                               ->GetProjectFilepath();
    Path gameProjectFilepath = dataDir.Append(projectFilepath.GetNameExt());
    File::Duplicate(projectFilepath, gameProjectFilepath);

    // Copy the Engine needed directories into the Data directory
    Path bangDataDir = dataDir.Append("Bang");
    Path bangAssetsDataDir = bangDataDir.Append("Assets");
    if (!File::CreateDir(bangDataDir))
    {
        return false;
    }

    if (!File::DuplicateDir(Paths::GetEngineAssetsDir(), bangAssetsDataDir))
    {
        Debug_Error("Could not duplicate engine assets directory '"
                    << Paths::GetEngineAssetsDir()
                    << "' into '"
                    << bangAssetsDataDir
                    << "'");
        return false;
    }

    // Copy the Project Assets in the Data directory
    Path gameDataAssetsDir = dataDir.Append("Assets");
    if (!File::DuplicateDir(Paths::GetProjectAssetsDir(), gameDataAssetsDir))
    {
        Debug_Error("Could not duplicate assets directory '"
                    << Paths::GetProjectAssetsDir()
                    << "' into '"
                    << gameDataAssetsDir
                    << "'");
        return false;
    }

    return true;
}

bool GameBuilder::CreateBehavioursLibrary(const Path &executableDir,
                                          BinType binType)
{
    // Create Libraries directory
    Path dataLibsDir = Path(executableDir).Append("Data").Append("Libraries");
    File::CreateDir(dataLibsDir);

    // Compile every behaviour into its .o
    Array<Path> behavioursSourceFiles = Paths::GetProjectAssetsDir().GetFiles(
        FindFlag::RECURSIVE, Extensions::GetSourceFileExtensions());

    // Preprocess behaviours
    for (const Path &behaviourSourcePath : behavioursSourceFiles)
    {
        Path behaviourHeader = behaviourSourcePath.WithExtension("h");
        BangPreprocessor::Preprocess(behaviourHeader);
    }

    EditorBehaviourManager *behaviourMgr = EditorBehaviourManager::GetActive();
    behaviourMgr->WaitForAsyncCompileJobs();

    // Compile
    for (const Path &behaviourSourcePath : behavioursSourceFiles)
    {
        Path outputObjPath = dataLibsDir.Append(behaviourSourcePath.GetName())
                                 .AppendExtension("o");

        Debug_Log("Compiling '" << behaviourSourcePath << "' into '"
                                << outputObjPath
                                << "'...");
        Compiler::Result res = behaviourMgr->CompileBehaviourObject(
            behaviourSourcePath, outputObjPath, binType);

        if (!outputObjPath.IsFile())
        {
            Debug_Error(res.output);
            return false;
        }
    }

    // Merge into dynamic/shared lib
    Array<Path> behaviourObjectsPaths =
        dataLibsDir.GetFiles(FindFlag::SIMPLE, {"o"});
    Path outputLibPath =
        dataLibsDir.Append("Behaviours")
            .AppendExtension(Extensions::GetDynamicLibExtension())
            .AppendExtension(String::ToString(Time::GetNow().GetMillis()));
    Debug_Log("Merging behaviour objects " << behaviourObjectsPaths << " into '"
                                           << outputLibPath
                                           << "'...");

    Compiler::Result res = behaviourMgr->MergeBehaviourObjects(
        behaviourObjectsPaths, outputLibPath, binType);
    if (!outputLibPath.IsFile())
    {
        Debug_Error(res.output);
        return false;
    }

    return true;
}
