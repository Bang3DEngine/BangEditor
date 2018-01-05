#ifndef BEHAVIOURCREATOR_H
#define BEHAVIOURCREATOR_H

#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class BehaviourCreator
{
public:
    BehaviourCreator() = delete;

    // Behaviour Creation
    static String GetNewBehaviourHeaderCode(const String &behaviourName);
    static String GetNewBehaviourSourceCode(const String &behaviourName);
    static Path GetNewBehaviourHeaderFilepath(const Path &dirPath,
                                              const String &behaviourName);
    static Path GetNewBehaviourSourceFilepath(const Path &dirPath,
                                              const String &behaviourName);
    static bool CanCreateNewBehaviour(const Path &dirPath,
                                      const String &behaviourName);
    static void CreateNewBehaviour(const Path &dirPath,
                                   const String &behaviourName,
                                   Path *behaviourHeaderPath,
                                   Path *behaviourSourcePath);

};

NAMESPACE_BANG_EDITOR_END

#endif // BEHAVIOURCREATOR_H

