#ifndef EDITORBEHAVIOURMANAGER_H
#define EDITORBEHAVIOURMANAGER_H

#include <queue>

#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/Mutex.h"
#include "Bang/BinType.h"
#include "Bang/Compiler.h"
#include "Bang/ThreadPool.h"
#include "Bang/BehaviourManager.h"

#include "BangEditor/BehaviourTracker.h"

NAMESPACE_BANG_BEGIN
FORWARD class Library;
FORWARD class Behaviour;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorBehaviourManager : public BehaviourManager
{
public:
    EditorBehaviourManager();
    virtual ~EditorBehaviourManager();

    void Update();

    void WaitForAsyncCompileJobs();
    bool PrepareBehavioursLibrary();
    bool IsCompiled(const Path& behaviourFilepath) const;
    bool IsCompiledWithError(const Path& behaviourFilepath) const;
    bool IsCompiledSuccessfully(const Path& behaviourFilepath) const;
    bool IsBeingCompiled(const Path& behaviourFilepath) const;
    bool AreAllBehavioursCompiled() const;
    bool IsSomeBehaviourBeingCompiled() const;
    bool AreAllBehavioursCompiledSuccessfully() const;
    bool IsBehavioursLibraryReady() const;
    static Behaviour* CreateBehaviourInstance(const String &behaviourName);
    static bool DeleteBehaviourInstance(const String &behaviourName,
                                        Behaviour *behaviour);

    virtual bool IsInstanceCreationAllowed() const override;

    static EditorBehaviourManager *GetActive();

private:
    ThreadPool m_compileThreadPool;
    BehaviourTracker m_behaviourTracker;

    Set<Path> m_compiledBehaviours;
    Set<Path> m_behavioursBeingCompiled;
    Set<Path> m_successfullyCompiledBehaviours;

    mutable Mutex m_mutex;
    std::queue<Compiler::Result> m_compileResults;

    void UpdateCompileInformations();

    Compiler::Result CompileBehaviourObject(const Path &behaviourPath);
    Compiler::Result CompileBehaviourObject(const Path& behaviourFilepath,
                                            const Path& outputObjectFilepath,
                                            BinType binaryType);
    void CompileBehaviourObjectAsync(const Path &behaviourPath);
    void CompileAllProjectBehaviours();
    void MergeIntoBehavioursLibrary();

    static Compiler::Result MergeBehaviourObjects(
                                      const List<Path> &behaviourObjectPaths,
                                      const Path &outputLibFilepath,
                                      BinType binaryType);

    static List<Path> GetBehaviourSourcesPaths();
    static Compiler::Job CreateBaseJob(BinType binaryType);
    static Path GetObjectOutputPath(const Path &inputBehaviourPath);
    static Compiler::Job CreateCompileBehaviourJob(const Path& behaviourFilepath,
                                                   const Path& outputObjectFilepath,
                                                   BinType binaryType);
    static void RemoveBehaviourLibrariesOf(const String& behaviourName);

    Mutex* GetMutex() const;
    BehaviourTracker *GetBehaviourTracker();
    const BehaviourTracker *GetBehaviourTracker() const;

    class BehaviourCompileRunnable : public ThreadRunnable
    {
    public:
        EditorBehaviourManager *m_behaviourManager = nullptr;
        Path m_behaviourPath = Path::Empty;
        void Run() override;
    };

    friend class GameBuilder;
    friend class BehaviourCompileRunnable;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORBEHAVIOURMANAGER_H
