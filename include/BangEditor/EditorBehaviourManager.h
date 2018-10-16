#ifndef EDITORBEHAVIOURMANAGER_H
#define EDITORBEHAVIOURMANAGER_H

#include <queue>
#include <vector>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/BehaviourManager.h"
#include "Bang/BinType.h"
#include "Bang/Compiler.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/FileTracker.h"
#include "Bang/IEvents.h"
#include "Bang/Map.h"
#include "Bang/Mutex.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Thread.h"
#include "Bang/ThreadPool.h"
#include "Bang/USet.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Behaviour;
FORWARD class Library;
FORWARD class IEventsFileTracker;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorBehaviourManager : public BehaviourManager,
                               public EventListener<IEventsFileTracker>
{
public:
    EditorBehaviourManager();
    virtual ~EditorBehaviourManager() override;

    void Update() override;

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

    USet<Path> m_compiledBehaviours;
    USet<Path> m_modifiedBehaviourPaths;
    USet<Path> m_behavioursBeingCompiled;
    USet<Path> m_successfullyCompiledBehaviours;

    mutable Mutex m_mutex;
    std::queue<Compiler::Result> m_compileResults;

    Compiler::Result CompileBehaviourObject(const Path &behaviourPath);
    Compiler::Result CompileBehaviourObject(const Path& behaviourFilepath,
                                            const Path& outputObjectFilepath,
                                            BinType binaryType);
    void CompileBehaviourObjectAsync(const Path &behaviourPath);
    void CompileAllProjectBehaviours();
    void MergeIntoBehavioursLibrary();

    static Compiler::Result MergeBehaviourObjects(
                                      const Array<Path> &behaviourObjectPaths,
                                      const Path &outputLibFilepath,
                                      BinType binaryType);

    static Array<Path> GetCompiledObjectsPaths();
    static Array<Path> GetBehaviourSourcesPaths();
    static Compiler::Job CreateBaseJob(BinType binaryType, bool addLibs);
    static Path GetObjectOutputPath(const Path &inputBehaviourPath);
    static Compiler::Job CreateCompileBehaviourJob(const Path& behaviourFilepath,
                                                   const Path& outputObjectFilepath,
                                                   BinType binaryType);
    static void RemoveBehaviourLibrariesOf(const String& behaviourName);
    static void RemoveOrphanBehaviourLibrariesAndObjects();

    Mutex* GetMutex() const;

    class BehaviourCompileRunnable : public ThreadRunnable
    {
    public:
        EditorBehaviourManager *m_behaviourManager = nullptr;
        Path m_behaviourPath = Path::Empty;
        void Run() override;
    };

    // IEventsFileTracker
    void OnPathAdded(const Path &path) override;
    void OnPathModified(const Path &path) override;
    void OnPathRemoved(const Path &path) override;

    friend class GameBuilder;
    friend class BehaviourCompileRunnable;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORBEHAVIOURMANAGER_H
