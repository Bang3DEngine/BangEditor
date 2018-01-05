#include "BangEditor/BehaviourManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/Extensions.h"
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

BehaviourManager::~BehaviourManager()
{
    if (m_behavioursLibrary) { delete m_behavioursLibrary; }
}

bool BehaviourManager::IsCompiled(const Path &behaviourFilepath)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (bm)
    {
        return bm->m_compiledBehaviours.ContainsKey(behaviourFilepath) &&
               bm->m_compiledBehaviours.Get(behaviourFilepath);
    }
    return false;
}

Library *BehaviourManager::GetBehavioursLibrary()
{
    BehaviourManager *bm = BehaviourManager::GetInstance();
    if (bm && bm->m_behavioursLibrary) { return bm->m_behavioursLibrary; }

    BehaviourManager::CompileAllProjectBehaviours();

    Path outputLibPath = EditorPaths::ProjectLibrariesDir().
                                      Append("Behaviours").
                                      AppendExtension("so").
                                      AppendExtension( String(Time::GetNow_Nanos()) );

    BehaviourManager::RemoveBehaviourLibrariesOf( "Behaviours" );

    List<Path> behaviourObjs = EditorPaths::ProjectLibrariesDir().
                               FindFiles(Path::FindFlag::Simple, {"o"});

    Compiler::Result mergeResult =
            BehaviourManager::MergeBehaviourObjects(outputLibPath,
                                                    behaviourObjs,
                                                    BinType::Debug);
    if (!outputLibPath.IsFile())
    {
        Debug_Error("Merge Behaviours library error: " << mergeResult.output);
        return nullptr;
    }

    Library *behavioursLibrary = new Library(outputLibPath);
    if (bm)
    {
        if (bm->m_behavioursLibrary) { delete bm->m_behavioursLibrary; }
        bm->m_behavioursLibrary = behavioursLibrary;
    }

    return behavioursLibrary;
}

Compiler::Result BehaviourManager::CompileBehaviourObject(
                                        const Path &behaviourFilepath,
                                        const Path &outputObjectFilepath,
                                        BinType binaryType)
{
    BehaviourManager *bm = BehaviourManager::GetInstance();

    File::CreateDirectory(outputObjectFilepath.GetDirectory());

    Compiler::Job job = BehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::Object;
    job.includePaths.PushBack( Paths::GetEngineIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetEditorIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetProjectIncludeDirs() );
    job.inputFiles.PushBack(behaviourFilepath);
    job.outputFile = outputObjectFilepath;

    BehaviourManager::RemoveBehaviourLibrariesOf(behaviourFilepath.GetName());
    Compiler::Result compileResult = Compiler::Compile(job);

    if (!outputObjectFilepath.IsFile())
    {
        Debug_Error("Behaviour '"  << behaviourFilepath.GetName() <<
                    "' did not compile correctly: " <<
                    compileResult.output);
    }

    if (bm)
    {
        bm->m_compiledBehaviours.Add(behaviourFilepath, true);
    }

    return compileResult;
}

void BehaviourManager::RemoveBehaviourLibrariesOf(const String& behaviourName)
{
    if (behaviourName.IsEmpty()) { return; }

    List<Path> libFilepaths = EditorPaths::ProjectLibrariesDir().
                                           FindFiles(Path::FindFlag::Simple);
    for (const Path &libFilepath : libFilepaths)
    {
        if (libFilepath.GetName() == behaviourName)
        {
            File::Remove(libFilepath);
        }
    }
}

void BehaviourManager::CompileBehaviourObject(const Path &behaviourPath)
{
    Path outputObjPath = EditorPaths::ProjectLibrariesDir().
                                      Append(behaviourPath.GetName()).
                                      AppendExtension("o").
                                      AppendExtension( String(Time::GetNow_Nanos()) );

    BehaviourManager::CompileBehaviourObject(behaviourPath,
                                             outputObjPath,
                                             BinType::Debug);
}

void BehaviourManager::CompileAllProjectBehaviours()
{
    List<Path> behaviourSources = EditorPaths::ProjectAssets().
                                  FindFiles(Path::FindFlag::Recursive,
                                            Extensions::GetSourceFileExtensions());
    for (const Path &behaviourSourcePath : behaviourSources)
    {
        BehaviourManager::CompileBehaviourObject(behaviourSourcePath);
    }
}

Compiler::Result BehaviourManager::MergeBehaviourObjects(
                                    const Path &outputLibFilepath,
                                    const List<Path> &behaviourObjectFilepaths,
                                    BinType binaryType)
{
    File::CreateDirectory(outputLibFilepath.GetDirectory());

    Compiler::Job job = BehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::SharedLib;
    job.inputFiles.PushBack(behaviourObjectFilepaths);
    job.outputFile = outputLibFilepath;

    return Compiler::Compile(job);
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


Behaviour *BehaviourManager::CreateBehaviourInstance(const String &behaviourName)
{
    Library *behavioursLib = BehaviourManager::GetBehavioursLibrary();
    if (!behavioursLib) { return nullptr; }

    if (!behavioursLib->IsLoaded()) { behavioursLib->Load(); }

    String errorString = "";
    if (behavioursLib->IsLoaded())
    {
        // Get the pointer to the CreateDynamically function
        String funcName = "CreateDynamically_" + behaviourName;
        Behaviour* (*createFunction)(Application*) =
            behavioursLib->Get< Behaviour*(*)(Application*) >(funcName.ToCString());

        if (createFunction)
        {
            // Call it and get the pointer to the created Behaviour
            // Create the Behaviour, passing to it the Application
            // of this main binary, so it can link it.
            return createFunction(Application::GetInstance());
        }
        else { errorString = behavioursLib->GetErrorString(); }
    }
    else { errorString = behavioursLib->GetErrorString(); }

    Debug_Error(errorString);
    return nullptr;
}

bool BehaviourManager::DeleteBehaviourInstance(const String &behaviourName,
                                               Behaviour *behaviour)
{
    Library *behavioursLibrary = BehaviourManager::GetBehavioursLibrary();
    if (!behavioursLibrary)
    {
        delete behaviour;
        return false;
    }

    if (!behavioursLibrary) { return false; }

    // Get the pointer to the DeleteDynamically function
    String funcName = "DeleteDinamically_" + behaviourName;
    void (*deleteFunction)(Behaviour*) =
            (behavioursLibrary->Get<void (*)(Behaviour*)>(funcName.ToCString()));

    if (deleteFunction)
    {
        deleteFunction(behaviour);
        return true;
    }

    Debug_Error(behavioursLibrary->GetErrorString());
    return false;
}

BehaviourManager *BehaviourManager::GetInstance()
{
    return EditorApplication::GetInstance()->GetBehaviourManager();
}
