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

    static bool IsCompiled(const Path& behaviourFilepath);
    static Library* GetBehaviourLib(const Path& behaviourFilepath);

    static void RemoveBehaviourLibrariesOf(const String& behaviourName);

private:
    Map<Path, bool> m_compiledBehaviours;
    Map<Path, Library*> m_cachedBehaviourLibraries;

    static Compiler::Result CompileBehaviourLib(
                                       const Path& behaviourFilepath,
                                       const Path& outputObjectFilepath,
                                       BinType binaryType);

    static Compiler::Job CreateBaseJob(BinType binaryType);

    static BehaviourManager* GetInstance();

    friend class GameBuilder;
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURMANAGER_H
