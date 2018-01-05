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

    static Library* CompileBehaviourLib(const Path& behaviourFilepath);
    static Compiler::Result CompileBehaviourLib(
                                       const Path& behaviourFilepath,
                                       const Path& outputObjectFilepath,
                                       BinType binaryType);

    static void RemoveBehaviourLibrariesOf(const String& behaviourName);

private:
    static Compiler::Job CreateBaseJob(BinType binaryType);

    static BehaviourManager* GetInstance();
};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURMANAGER_H
