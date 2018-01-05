#include "BangEditor/BehaviourManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/Application.h"

#include "BangEditor/Project.h"
#include "BangEditor/Behaviour.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorApplication.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

BehaviourManager::BehaviourManager()
{
}

Library *BehaviourManager::CompileBehaviourLib(const Path &behaviourFilepath)
{
    Path outputLibPath = EditorPaths::ProjectLibrariesDir().
                                      Append(behaviourFilepath.GetName()).
                                      AppendExtension("so").
                                      AppendExtension( String(Time::GetNow_Nanos()) );

    BehaviourManager::CompileBehaviourLib(behaviourFilepath,
                                             outputLibPath,
                                             BinType::Debug);

    if (!outputLibPath.IsFile()) { return nullptr; }

    Library *behaviourLibrary = new Library(outputLibPath);
    return behaviourLibrary;
}

Compiler::Result BehaviourManager::CompileBehaviourLib(
                                        const Path &behaviourFilepath,
                                        const Path &outputObjectFilepath,
                                        BinType binaryType)
{
    File::Remove(outputObjectFilepath);
    File::CreateDirectory(outputObjectFilepath.GetDirectory());

    Compiler::Job job = BehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::SharedLib;
    job.includePaths.PushBack( Paths::GetEngineIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetEditorIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetProjectIncludeDirs() );
    job.inputFiles.PushBack(behaviourFilepath);
    job.outputFile = outputObjectFilepath;

    return Compiler::Compile(job);
}

void BehaviourManager::RemoveBehaviourLibrariesOf(const String& behaviourName)
{
    if (behaviourName.IsEmpty()) { return; }

    List<Path> libFilepaths = EditorPaths::ProjectLibrariesDir().
                                           FindFiles(Path::FindFlag::Simple);
    for (const Path &libFilepath : libFilepaths)
    {
        if (libFilepath.GetAbsolute().Contains(".so.") &&
            libFilepath.GetName() == behaviourName)
        {
            File::Remove(libFilepath);
        }
    }
}

Compiler::Job BehaviourManager::CreateBaseJob(BinType binaryType)
{
    Compiler::Job job;
    job.libDirs.PushBack(Paths::EngineLibrariesDir(binaryType));
    job.libraries.PushBack( List<String>({"Bang"}) );

    job.flags =  {"-fPIC", "--std=c++11"};
    if (binaryType == BinType::Debug)
    {
        job.flags.PushBack( List<String>({"-O0", "-g", "-Wl,-O0,--export-dynamic"}) );
    }
    else
    {
        job.flags.PushBack( List<String>({"-O3", "-Wl,-O3,--export-dynamic"}) );
    }

    return job;
}

BehaviourManager *BehaviourManager::GetInstance()
{
    return EditorApplication::GetInstance()->GetBehaviourManager();
}
