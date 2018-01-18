#include "BangEditor/EditorBehaviourManager.h"

#include "Bang/File.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/Behaviour.h"
#include "Bang/Extensions.h"
#include "Bang/Application.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Project.h"
#include "BangEditor/EditorPaths.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorBehaviourManager::EditorBehaviourManager()
{
    m_compileThreadPool.SetMaxThreadCount(1);
    m_compileThreadPool.SetName("BehaviourCompileThread");
}

EditorBehaviourManager::~EditorBehaviourManager()
{
    if (m_behavioursLibrary) { delete m_behavioursLibrary; }
}

void EditorBehaviourManager::Update()
{
    UpdateCompileInformations();

    if (Editor::IsEditingScene())
    {
        List<Path> behaviourPaths = GetBehaviourSourcesPaths();
        for (const Path &behaviourPath : behaviourPaths)
        {
            if (!IsCompiled(behaviourPath) && !IsBeingCompiled(behaviourPath))
            {
                CompileBehaviourObjectAsync(behaviourPath);
            }
        }
    }
}

void EditorBehaviourManager::WaitForAsyncCompileJobs()
{
    while (IsSomeBehaviourBeingCompiled()) { Thread::SleepCurrentThread(0.2f); }
}

bool EditorBehaviourManager::PrepareBehavioursLibrary()
{
    WaitForAsyncCompileJobs();

    // Compile
    GetBehaviourTracker()->CheckForChanges();
    UpdateCompileInformations();
    if (!AreAllBehavioursCompiledSuccessfully())
    {
        CompileAllProjectBehaviours();
        if (!AreAllBehavioursCompiledSuccessfully())
        {
            Debug_Error("Can not prepare behaviours library. "
                        "Could not compile all behaviours.");
            return false;
        }
    }

    // Merge
    if (!IsBehavioursLibraryReady())
    {
        MergeIntoBehavioursLibrary();
        if (!IsBehavioursLibraryReady())
        {
            Debug_Error("Can not prepare behaviours library. "
                        "Could not link all behaviours into behaviours library.");
            return false;
        }
    }

    return true;
}

bool EditorBehaviourManager::AreAllBehavioursCompiled() const
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

bool EditorBehaviourManager::IsSomeBehaviourBeingCompiled() const
{
    MutexLocker ml( GetMutex() ); (void)ml;
    return !m_behavioursBeingCompiled.IsEmpty();
}

bool EditorBehaviourManager::AreAllBehavioursCompiledSuccessfully() const
{
    List<Path> behaviourPaths = GetBehaviourSourcesPaths();
    for (const Path &behaviourPath : behaviourPaths)
    {
        if (IsBeingCompiled(behaviourPath) || !IsCompiledSuccessfully(behaviourPath))
        {
            return false;
        }
    }
    return true;
}

bool EditorBehaviourManager::IsBehavioursLibraryReady() const
{
    return (GetBehavioursLibrary() != nullptr);
}

bool EditorBehaviourManager::IsCompiled(const Path &behaviourFilepath) const
{
    MutexLocker ml( GetMutex() ); (void)ml;
    return m_compiledBehaviours.Contains(behaviourFilepath);
}

bool EditorBehaviourManager::IsCompiledWithError(const Path &behaviourFilepath) const
{
    const bool isCompiled = IsCompiled(behaviourFilepath);

    MutexLocker ml( GetMutex() ); (void)ml;
    return isCompiled && !m_successfullyCompiledBehaviours.Contains(behaviourFilepath);
}

bool EditorBehaviourManager::IsCompiledSuccessfully(const Path &behaviourFilepath) const
{
    const bool isCompiled = IsCompiled(behaviourFilepath);

    MutexLocker ml( GetMutex() ); (void)ml;
    return isCompiled && m_successfullyCompiledBehaviours.Contains(behaviourFilepath);
}

bool EditorBehaviourManager::IsBeingCompiled(const Path &behaviourFilepath) const
{
    MutexLocker ml( GetMutex() ); (void)ml;
    return m_behavioursBeingCompiled.Contains(behaviourFilepath);
}

Library *EditorBehaviourManager::GetBehavioursLibrary() const
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
    if (outputObjectFilepath.IsFile())
    {
        m_successfullyCompiledBehaviours.Add(behaviourFilepath);
    }
    else { m_successfullyCompiledBehaviours.Remove(behaviourFilepath); }
    m_behavioursBeingCompiled.Remove(behaviourFilepath);
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
        m_behavioursBeingCompiled.Add(behaviourPath);
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

    List<Path> behaviourObjs = EditorPaths::GetProjectLibrariesDir().
                               GetFiles(Path::FindFlag::Simple, {"o"});

    EditorBehaviourManager::RemoveBehaviourLibrariesOf( "Behaviours" );
    Compiler::Result mergeResult = MergeBehaviourObjects(behaviourObjs,
                                                         outputLibPath,
                                                         BinType::Debug);
    if (outputLibPath.IsFile())
    {
        Library *behavioursLibrary = new Library(outputLibPath);
        SetBehavioursLibrary(behavioursLibrary);
    }
    else
    {
        Debug_Error("Merge Behaviours library error: " << mergeResult.output);
    }
}

void EditorBehaviourManager::SetBehavioursLibrary(Library *behavioursLibrary)
{
    if (GetBehavioursLibrary()) { delete GetBehavioursLibrary(); }
    m_behavioursLibrary = behavioursLibrary;
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
    GetBehaviourTracker()->Update();

    GetMutex()->Lock();
    List<Path> modifiedBehaviours;
    for (const Path &compiledBehaviour : m_compiledBehaviours)
    {
        if ( GetBehaviourTracker()->HasBeenModified(compiledBehaviour) )
        {
            Debug_Log("Modified " << compiledBehaviour);
            SetBehavioursLibrary(nullptr);
            modifiedBehaviours.PushBack(compiledBehaviour);
        }
    }
    GetMutex()->UnLock();

    for (const Path &modifiedBehaviour : modifiedBehaviours)
    {
        if ( !IsBeingCompiled(modifiedBehaviour) )
        {
            MutexLocker ml(GetMutex()); (void)ml;
            m_compiledBehaviours.Remove(modifiedBehaviour);
            m_successfullyCompiledBehaviours.Remove(modifiedBehaviour);
        }
    }
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

    const Path &outputPath = compileResult.compileJob.outputFile;
    if (!outputPath.IsFile())
    {
        Debug_Error("Behaviour '"  << outputPath.GetName() <<
                    "' did not compile correctly: " <<
                    compileResult.output);
    }
}
