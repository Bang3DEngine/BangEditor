#include "BangEditor/EditorBehaviourManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/Behaviour.h"
#include "Bang/Extensions.h"
#include "Bang/Application.h"

#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorBehaviourManager::EditorBehaviourManager()
{
}

EditorBehaviourManager::~EditorBehaviourManager()
{
    if (m_behavioursLibrary) { delete m_behavioursLibrary; }
}

void EditorBehaviourManager::Update()
{
    GetBehaviourTracker()->Update();
    UpdateCompiledPathsSet();
}

bool EditorBehaviourManager::IsCompiled(const Path &behaviourFilepath)
{
    EditorBehaviourManager *bm = EditorBehaviourManager::GetInstance();
    if (bm)
    {
        return bm->m_compiledBehaviours.Contains(behaviourFilepath);
    }
    return false;
}

Library *EditorBehaviourManager::GetBehavioursLibrary()
{
    EditorBehaviourManager *bm = EditorBehaviourManager::GetInstance();
    if (bm && bm->m_behavioursLibrary)
    {
        bool allBehavioursCompiled = true;
        List<Path> behaviourSourcePaths = EditorBehaviourManager::GetBehaviourSourcesPaths();
        for (const Path &behaviourSourcePath : behaviourSourcePaths)
        {
            if (!EditorBehaviourManager::IsCompiled(behaviourSourcePath))
            {
                allBehavioursCompiled = false;
                break;
            }
        }
        if (allBehavioursCompiled) { return bm->m_behavioursLibrary; }
    }

    EditorBehaviourManager::CompileAllProjectBehaviours();

    Path outputLibPath = EditorPaths::GetProjectLibrariesDir().
                                      Append("Behaviours").
                                      AppendExtension("so").
                                      AppendExtension( String(Time::GetNow_Nanos()) );

    EditorBehaviourManager::RemoveBehaviourLibrariesOf( "Behaviours" );

    List<Path> behaviourObjs = EditorPaths::GetProjectLibrariesDir().
                               GetFiles(Path::FindFlag::Simple, {"o"});

    Compiler::Result mergeResult =
            EditorBehaviourManager::MergeBehaviourObjects(behaviourObjs,
                                                          outputLibPath,
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

Behaviour *EditorBehaviourManager::CreateBehaviourInstance(const String &behaviourName)
{
    Library *behavioursLib = EditorBehaviourManager::GetBehavioursLibrary();
    return BehaviourManager::CreateBehaviourInstance(behaviourName, behavioursLib);
}

bool EditorBehaviourManager::DeleteBehaviourInstance(const String &behaviourName,
                                                     Behaviour *behaviour)
{
    Library *behavioursLib = EditorBehaviourManager::GetBehavioursLibrary();
    return BehaviourManager::DeleteBehaviourInstance(behaviourName,
                                                     behaviour,
                                                     behavioursLib);
}

Compiler::Result EditorBehaviourManager::CompileBehaviourObject(
                                        const Path &behaviourFilepath,
                                        const Path &outputObjectFilepath,
                                        BinType binaryType)
{
    EditorBehaviourManager *bm = EditorBehaviourManager::GetInstance();

    File::CreateDirectory(outputObjectFilepath.GetDirectory());

    Compiler::Job job = EditorBehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::Object;
    job.includePaths.PushBack( Paths::GetEngineIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetEditorIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetProjectIncludeDirs() );
    job.inputFiles.PushBack(behaviourFilepath);
    job.outputFile = outputObjectFilepath;

    EditorBehaviourManager::RemoveBehaviourLibrariesOf(behaviourFilepath.GetName());
    Compiler::Result compileResult = Compiler::Compile(job);

    if (!outputObjectFilepath.IsFile())
    {
        Debug_Error("Behaviour '"  << behaviourFilepath.GetName() <<
                    "' did not compile correctly: " <<
                    compileResult.output);
    }

    if (bm)
    {
        bm->m_compiledBehaviours.Add(behaviourFilepath);
    }

    return compileResult;
}

void EditorBehaviourManager::RemoveBehaviourLibrariesOf(const String& behaviourName)
{
    if (behaviourName.IsEmpty()) { return; }

    List<Path> libFilepaths = EditorPaths::GetProjectLibrariesDir().
                                           GetFiles(Path::FindFlag::Simple);
    for (const Path &libFilepath : libFilepaths)
    {
        if (libFilepath.GetName() == behaviourName)
        {
            File::Remove(libFilepath);
        }
    }
}

void EditorBehaviourManager::CompileBehaviourObject(const Path &behaviourPath)
{
    Path outputObjPath = EditorPaths::GetProjectLibrariesDir().
                                      Append(behaviourPath.GetName()).
                                      AppendExtension("o").
                                      AppendExtension( String(Time::GetNow_Nanos()) );

    EditorBehaviourManager::CompileBehaviourObject(behaviourPath,
                                             outputObjPath,
                                             BinType::Debug);
}

void EditorBehaviourManager::CompileAllProjectBehaviours()
{
    EditorBehaviourManager *bm = EditorBehaviourManager::GetInstance();
    bm->UpdateCompiledPathsSet();

    List<Path> behaviourSources = EditorBehaviourManager::GetBehaviourSourcesPaths();
    for (const Path &behaviourSourcePath : behaviourSources)
    {
        EditorBehaviourManager::CompileBehaviourObject(behaviourSourcePath);
    }
}

Compiler::Result EditorBehaviourManager::MergeBehaviourObjects(
                                    const List<Path> &behaviourObjectFilepaths,
                                    const Path &outputLibFilepath,
                                    BinType binaryType)
{
    File::CreateDirectory(outputLibFilepath.GetDirectory());

    Compiler::Job job = EditorBehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::SharedLib;
    job.inputFiles.PushBack(behaviourObjectFilepaths);
    job.outputFile = outputLibFilepath;

    return Compiler::Compile(job);
}

List<Path> EditorBehaviourManager::GetBehaviourSourcesPaths()
{
    List<Path> behaviourSources = EditorPaths::GetProjectAssetsDir().
                                  GetFiles(Path::FindFlag::Recursive,
                                           Extensions::GetSourceFileExtensions());
    return behaviourSources;
}

Compiler::Job EditorBehaviourManager::CreateBaseJob(BinType binaryType)
{
    Compiler::Job job;
    job.libDirs.PushBack(Paths::GetEngineLibrariesDir(binaryType));
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

void EditorBehaviourManager::UpdateCompiledPathsSet()
{
    for (const Path &compiledBehaviour : m_compiledBehaviours)
    {
        if (GetBehaviourTracker()->HasBeenModified(compiledBehaviour))
        {
            m_compiledBehaviours.Remove(compiledBehaviour);
        }
    }
}

BehaviourTracker *EditorBehaviourManager::GetBehaviourTracker()
{ return &m_behaviourTracker; }
const BehaviourTracker *EditorBehaviourManager::GetBehaviourTracker() const
{ return &m_behaviourTracker; }

EditorBehaviourManager *EditorBehaviourManager::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetBehaviourManager();
}
