#ifndef BEHAVIOURCREATOR_H
#define BEHAVIOURCREATOR_H

#include "Bang/BangDefines.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
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
}

#endif  // BEHAVIOURCREATOR_H
