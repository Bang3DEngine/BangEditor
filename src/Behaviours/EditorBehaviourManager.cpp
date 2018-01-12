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
    m_compileThreadPool.SetMaxThreadCount(3);
    m_compileThreadPool.SetName("BehaviourCompileThread");
}

EditorBehaviourManager::~EditorBehaviourManager()
{
    if (m_behavioursLibrary) { delete m_behavioursLibrary; }
}

void EditorBehaviourManager::Update()
{
    GetBehaviourTracker()->Update();
    ProcessEnqueuedCompileResults();
    UpdateCompileInformations();

    List<Path> behaviourPaths = GetBehaviourSourcesPaths();
    for (const Path &behaviourPath : behaviourPaths)
    {
        if (!IsCompiled(behaviourPath) && !IsBeingCompiled(behaviourPath))
        {
            CompileBehaviourObjectAsync(behaviourPath);
        }
    }
}

void EditorBehaviourManager::PrepareBehavioursLibrary()
{
    // Compile
    do
    {
        GetBehaviourTracker()->CheckForChanges();
        CompileAllProjectBehaviours();
    }
    while (!AreAllBehavioursReady());

    // Merge
    while(!IsBehavioursLibraryReady())
    {
        MergeIntoBehavioursLibrary();
    }
}

bool EditorBehaviourManager::AreAllBehavioursReady() const
{
    List<Path> behaviourPaths = GetBehaviourSourcesPaths();
    for (const Path &behaviourPath : behaviourPaths)
    {
        if (IsBeingCompiled(behaviourPath) || !IsCompiled(behaviourPath))
        {
            return false;
        }
    }
    return true;
}

bool EditorBehaviourManager::IsBehavioursLibraryReady() const
{
    return m_behavioursLibraryReady;
}

bool EditorBehaviourManager::IsCompiled(const Path &behaviourFilepath) const
{
    MutexLocker ml( GetMutex() ); (void)ml;
    return m_compiledBehaviours.Contains(behaviourFilepath);
}

bool EditorBehaviourManager::IsBeingCompiled(const Path &behaviourFilepath) const
{
    MutexLocker ml( GetMutex() );
    return m_behavioursBeingCompiledAsync.Contains(behaviourFilepath);
}

Library *EditorBehaviourManager::GetBehavioursLibrary()
{
    return m_behavioursLibrary;
}

Behaviour *EditorBehaviourManager::CreateBehaviourInstance(const String &behaviourName)
{
    Library *behavioursLib = EditorBehaviourManager::GetInstance()->GetBehavioursLibrary();
    return BehaviourManager::CreateBehaviourInstance(behaviourName, behavioursLib);
}

bool EditorBehaviourManager::DeleteBehaviourInstance(const String &behaviourName,
                                                     Behaviour *behaviour)
{
    Library *behavioursLib = EditorBehaviourManager::GetInstance()->GetBehavioursLibrary();
    return BehaviourManager::DeleteBehaviourInstance(behaviourName,
                                                     behaviour,
                                                     behavioursLib);
}


Compiler::Result EditorBehaviourManager::CompileBehaviourObject(const Path &behaviourPath)
{
    Path outputObjPath = GetObjectOutputPath(behaviourPath);
    return EditorBehaviourManager::CompileBehaviourObject(behaviourPath,
                                                          outputObjPath,
                                                          BinType::Debug);
}
Compiler::Result EditorBehaviourManager::CompileBehaviourObject(
                                        const Path &behaviourFilepath,
                                        const Path &outputObjectFilepath,
                                        BinType binaryType)
{
    EditorBehaviourManager::RemoveBehaviourLibrariesOf(outputObjectFilepath.GetName());
    File::CreateDirectory(outputObjectFilepath.GetDirectory());

    Compiler::Job job =
         EditorBehaviourManager::CreateCompileBehaviourJob(behaviourFilepath,
                                                           outputObjectFilepath,
                                                           binaryType);


    Compiler::Result compileResult = Compiler::Compile(job);

    GetMutex()->Lock();
    m_compiledBehaviours.Add(behaviourFilepath);
    GetMutex()->UnLock();

    return compileResult;
}

void EditorBehaviourManager::CompileBehaviourObjectAsync(const Path &behaviourPath)
{
    BehaviourCompileRunnable *compileRunnable = new BehaviourCompileRunnable();
    compileRunnable->m_behaviourManager = this;
    compileRunnable->m_behaviourPath = behaviourPath;

    bool compilingThreadStarted = m_compileThreadPool.TryStart(compileRunnable);
    if (compilingThreadStarted)
    {
        MutexLocker ml(GetMutex()); (void)ml;
        m_behavioursBeingCompiledAsync.Add(behaviourPath);
    }
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

void EditorBehaviourManager::CompileAllProjectBehaviours()
{
    UpdateCompileInformations();
    ProcessEnqueuedCompileResults();

    List<Path> behaviourSources = EditorBehaviourManager::GetBehaviourSourcesPaths();
    for (const Path &behaviourSourcePath : behaviourSources)
    {
        if (!IsCompiled(behaviourSourcePath) &&
            !IsBeingCompiled(behaviourSourcePath))
        {
            EditorBehaviourManager::CompileBehaviourObject(behaviourSourcePath);
        }
    }
}

void EditorBehaviourManager::MergeIntoBehavioursLibrary()
{
    Path outputLibPath = EditorPaths::GetProjectLibrariesDir().
                              Append("Behaviours").
                              AppendExtension("so").
                              AppendExtension( String(Time::GetNow_Nanos()) );

    if (!IsBehavioursLibraryReady())
    {
        EditorBehaviourManager::RemoveBehaviourLibrariesOf( "Behaviours" );

        List<Path> behaviourObjs = EditorPaths::GetProjectLibrariesDir().
                                   GetFiles(Path::FindFlag::Simple, {"o"});

        Compiler::Result mergeResult = MergeBehaviourObjects(behaviourObjs,
                                                             outputLibPath,
                                                             BinType::Debug);
        if (!outputLibPath.IsFile())
        {
            Debug_Error("Merge Behaviours library error: " << mergeResult.output);
            return;
        }

        Library *behavioursLibrary = new Library(outputLibPath);
        SetBehavioursLibrary(behavioursLibrary);
    }
}

void EditorBehaviourManager::SetBehavioursLibrary(Library *behavioursLibrary)
{
    if (m_behavioursLibrary) { delete m_behavioursLibrary; }
    m_behavioursLibrary = behavioursLibrary;
    m_behavioursLibraryReady = (behavioursLibrary != nullptr);
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

Path EditorBehaviourManager::GetObjectOutputPath(const Path &inputBehaviourPath)
{
    return EditorPaths::GetProjectLibrariesDir().
                              Append(inputBehaviourPath.GetName()).
                              AppendExtension("o").
                              AppendExtension( String(Time::GetNow_Nanos()) );
}

Compiler::Job EditorBehaviourManager::CreateCompileBehaviourJob(
                                            const Path &behaviourFilepath,
                                            const Path &outputObjectFilepath,
                                            BinType binaryType)
{
    Compiler::Job job = EditorBehaviourManager::CreateBaseJob(binaryType);
    job.outputMode = Compiler::OutputType::Object;
    job.includePaths.PushBack( Paths::GetEngineIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetEditorIncludeDirs() );
    job.includePaths.PushBack( EditorPaths::GetProjectIncludeDirs() );
    job.inputFiles.PushBack(behaviourFilepath);
    job.outputFile = outputObjectFilepath;

    return job;
}

void EditorBehaviourManager::UpdateCompileInformations()
{
    MutexLocker ml( GetMutex() ); (void)ml;
    for (const Path &compiledBehaviour : m_compiledBehaviours)
    {
        if (GetBehaviourTracker()->HasBeenModified(compiledBehaviour))
        {
            m_behavioursLibraryReady = false;
            m_compiledBehaviours.Remove(compiledBehaviour);
        }
    }
}

void EditorBehaviourManager::ProcessEnqueuedCompileResults()
{
    MutexLocker ml(GetMutex()); (void)ml;
    while (!m_compileResults.empty())
    {
        const Compiler::Result &result = m_compileResults.front();
        if (result.compileJob.outputFile.IsFile())
        {
            const Path &behaviourPath = result.compileJob.inputFiles.Front();
            m_compiledBehaviours.Add(behaviourPath);
            m_behavioursBeingCompiledAsync.Remove(behaviourPath);
            Debug_Log("Compiled " << behaviourPath);
        }
        m_compileResults.pop();
    }
    ASSERT(m_compileResults.empty());
}

void EditorBehaviourManager::EnqueueCompileResult(
                                    const Compiler::Result &compileResult)
{
    MutexLocker ml( GetMutex() ); (void)ml;
    m_compileResults.push(compileResult);
}

Mutex* EditorBehaviourManager::GetMutex() const { return &m_mutex; }
BehaviourTracker *EditorBehaviourManager::GetBehaviourTracker()
{ return &m_behaviourTracker; }
const BehaviourTracker *EditorBehaviourManager::GetBehaviourTracker() const
{ return &m_behaviourTracker; }

EditorBehaviourManager *EditorBehaviourManager::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetBehaviourManager();
}

void EditorBehaviourManager::BehaviourCompileRunnable::Run()
{
    Compiler::Result compileResult =
                m_behaviourManager->CompileBehaviourObject(m_behaviourPath);
    m_behaviourManager->EnqueueCompileResult(compileResult);

    const Path &outputPath = compileResult.compileJob.outputFile;
    if (!outputPath.IsFile())
    {
        Debug_Error("Behaviour '"  << outputPath.GetName() <<
                    "' did not compile correctly: " <<
                    compileResult.output);
    }
}
