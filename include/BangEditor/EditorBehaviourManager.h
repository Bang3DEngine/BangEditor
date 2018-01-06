#ifndef EDITORBEHAVIOURMANAGER_H
#define EDITORBEHAVIOURMANAGER_H

#include "Bang/Map.h"
#include "Bang/Path.h"
#include "Bang/BinType.h"
#include "Bang/Compiler.h"
#include "Bang/BehaviourManager.h"

#include "BangEditor/BangEditor.h"

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

    static Compiler::Result MergeBehaviourObjects(const List<Path> &behaviourObjectPaths,
                                                  const Path &outputLibFilepath,
                                                  BinType binaryType);

    static Compiler::Job CreateBaseJob(BinType binaryType);

    static EditorBehaviourManager* GetInstance();

    friend class GameBuilder;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORBEHAVIOURMANAGER_H
