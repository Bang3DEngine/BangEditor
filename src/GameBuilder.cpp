#include "BangEditor/GameBuilder.h"

#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Extensions.h"
#include "Bang/SystemProcess.h"
#include "Bang/BangPreprocessor.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/EditorBehaviourManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

Path GameBuilder::GetExecutablePath()
{
    Project *project = ProjectManager::GetCurrentProject();
    Path outputExecutableFilepath = Paths::GetProjectDir().
                                         Append(project->GetProjectName() + "_Game").
                                         Append(project->GetProjectName()).
                                         AppendExtension("exe");
    return outputExecutableFilepath;
}

void GameBuilder::BuildGame()
{
    Path gameExecPath = GameBuilder::GetExecutablePath();
    File::CreateDirectory(gameExecPath.GetDirectory());

    GameBuilder::BuildGame(gameExecPath.GetName(),
                           gameExecPath.GetDirectory(),
                           // BinType::Debug,
                           BinType::Release,
                           true);
}

void GameBuilder::BuildGame(const String &gameName,
                            const Path &gameDir,
                            BinType binaryType,
                            bool compileBehaviours)
{
    Debug_Log("Compiling the game executable...");
    if (!GameBuilder::CompileGameExecutable(binaryType))
    {
        Debug_Error("Could not compile game executable");
        return;
    }

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
            Debug_Error("Could not compile the behaviours");
            return;
        }
    }

    Path gameExecutablePath = gameDir.Append(gameName).AppendExtension("exe");
    Debug_Log("Moving the executable to '" << gameExecutablePath  << "'...");
    const Path originalGameOutputDir = EditorPaths::GetExecutableDir();
                // EditorPaths::GetGameExecutableOutputFile(binaryType);

    File::Remove(gameExecutablePath); // Remove old game, if any
    File::Rename(originalGameOutputDir, gameExecutablePath);

    Debug_Log("Build finished successfully!");
}

bool GameBuilder::CompileGameExecutable(BinType binaryType)
{
    List<Path> sceneFiles = Paths::GetProjectAssetsDir()
                                    .GetFiles(Path::FindFlag::Recursive,
                                               {Extensions::GetSceneExtension()});
    if (sceneFiles.IsEmpty())
    {
        Debug_Error("Please save at least one scene in the "
                     "Assets directory to build the game");
        return false;
    }

    const Path gameOutputFilepath = EditorPaths::GetExecutableDir();
                      // EditorPaths::GetGameExecutableOutputFile(binaryType);
    // File::Remove(gameOutputFilepath);

    String debugOrRelease = (binaryType == BinType::Debug) ? "Debug" : "Release";
    String cmd = "cd " + EditorPaths::GetExecutableDir().GetAbsolute() + " ; " +
                 " mkdir a ; mkdir b " + debugOrRelease;

    Debug_Log("Compiling game executable with: " << cmd);

    SystemProcess process;
    process.Start(cmd);

    String out = "";
    while (!process.WaitUntilFinished(0.1f))
    {
        String partialOut = process.ReadStandardOutput() +
                            process.ReadStandardError();
        if (!partialOut.IsEmpty())
        {
            out += partialOut;
            Debug_Log(partialOut);
        }
    }

    out += process.ReadStandardOutput() +
           process.ReadStandardError();
    process.Close();

    Debug_Peek(gameOutputFilepath);
    if (!gameOutputFilepath.IsFile())
    {
        Debug_Error(out);
        return false;
    }
    return true;
}

bool GameBuilder::CreateDataDirectory(const Path &executableDir)
{
    Path dataDir = executableDir.Append("Data");
    File::Remove(dataDir);
    if (!File::CreateDirectory(dataDir)) { return false; }

    // Copy the Engine Assets in the Data directory
    Path gameDataEngineAssetsDir = dataDir.Append("Assets");
    if (!File::CreateDirectory(gameDataEngineAssetsDir)) { return false; }
    if (!File::DuplicateDir(Paths::GetEngineAssetsDir(), gameDataEngineAssetsDir))
    {
        Debug_Error("Could not duplicate engine assets directory '" <<
                    Paths::GetEngineAssetsDir() << "' into '" << gameDataEngineAssetsDir << "'");
        return false;
    }

    // Copy the Project Assets in the Data directory
    Path gameDataAssetsDir = dataDir.Append("Assets");
    if (!File::DuplicateDir(Paths::GetProjectAssetsDir(), gameDataAssetsDir))
    {
        Debug_Error("Could not duplicate assets directory '" <<
                    Paths::GetProjectAssetsDir() << "' into '" <<
                    gameDataAssetsDir << "'");
        return false;
    }

    return true;
}

bool GameBuilder::CreateBehavioursLibrary(const Path &executableDir,
                                          BinType binType)
{
    // Create Libraries directory
    Path dataLibsDir = Path(executableDir).Append("Data").Append("Libraries");
    File::CreateDirectory(dataLibsDir);

    // Compile every behaviour into its .o
    List<Path> behavioursSourceFiles = Paths::GetProjectAssetsDir()
                                        .GetFiles(Path::FindFlag::Recursive,
                                         Extensions::GetSourceFileExtensions());

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

        Debug_Log("Compiling '" << behaviourSourcePath << "' into '" <<
                  outputObjPath << "'...");
        Compiler::Result res = behaviourMgr->CompileBehaviourObject(behaviourSourcePath,
                                                                    outputObjPath,
                                                                    binType);

        if (!outputObjPath.IsFile()) { Debug_Error(res.output); return false; }
    }

    // Merge into .so
    List<Path> behaviourObjectsPaths = dataLibsDir.GetFiles(Path::FindFlag::Simple,
                                                         {"o"});
    Path outputLibPath =
                dataLibsDir.Append("Behaviours")
                       .AppendExtension("so")
                       .AppendExtension( String::ToString(Time::GetNow_Millis()) );
    Debug_Log("Merging behaviour objects " << behaviourObjectsPaths <<
              " into '" << outputLibPath << "'...");

    Compiler::Result res = behaviourMgr->MergeBehaviourObjects(behaviourObjectsPaths,
                                                               outputLibPath,
                                                               binType);
    if (!outputLibPath.IsFile()) { Debug_Error(res.output); return false; }

    return true;
}
