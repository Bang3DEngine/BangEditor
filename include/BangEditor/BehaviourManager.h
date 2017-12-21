#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

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

    static Compiler::Result CompileBehaviourObject(
                                       const Path& behaviourFilepath,
                                       const Path& outputObjectFilepath,
                                       BinType binaryType);

    static Compiler::Result MergeBehaviourObjects(
                                    const Path& outputLibFilepath,
                                    const List<Path> &behaviourObjectFilepaths,
                                    BinType binaryType);

    static Library *GetBehavioursLibrary();
    static void LoadBehavioursLibrary(const Path &behavioursLibrary);

    static void RemoveOldBehaviourLibraries(const Path& librariesDir);

private:
    Library *m_behavioursLibrary = nullptr;

    static Compiler::Job CreateBaseJob(BinType binaryType);

    static BehaviourManager* GetInstance();
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURMANAGER_H
