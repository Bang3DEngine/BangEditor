#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/BinType.h"
#include "Bang/Compiler.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Library;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Behaviour;

class BehaviourManager
{
public:
    BehaviourManager();
    virtual ~BehaviourManager();

    static bool IsCompiled(const Path& behaviourFilepath);
    static Library* GetBehavioursLibrary();

    static Behaviour* CreateBehaviourInstance(const String &behaviourName);
    static bool DeleteBehaviourInstance(const String &behaviourName,
                                        Behaviour *behaviour);

private:
    Library *m_behavioursLibrary = nullptr;
    Map<Path, bool> m_compiledBehaviours;
    Map<Path, Library*> m_cachedBehaviourLibraries;

    static void RemoveBehaviourLibrariesOf(const String& behaviourName);

    static void CompileBehaviourObject(const Path &behaviourPath);
    static Compiler::Result CompileBehaviourObject(
                                       const Path& behaviourFilepath,
                                       const Path& outputObjectFilepath,
                                       BinType binaryType);

    static void CompileAllProjectBehaviours();

    static Compiler::Result MergeBehaviourObjects(const Path &outputLibFilepath,
                                                  const List<Path> &behaviourObjectPaths,
                                                  BinType binaryType);

    static Compiler::Job CreateBaseJob(BinType binaryType);

    static BehaviourManager* GetInstance();

    friend class GameBuilder;
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURMANAGER_H
