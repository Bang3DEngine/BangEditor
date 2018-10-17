#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include "Bang/BangDefines.h"
#include "Bang/BinType.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

using namespace Bang;
namespace BangEditor
{
class GameBuilder
{
public:
    static void BuildGame();
    static void BuildGame(const String &gameName,
                          const Path &gameDir,
                          BinType binType,
                          bool compileBehaviours = true);

    static Path GetExecutablePath();

    GameBuilder() = delete;

private:
    static bool CompileGameExecutable(BinType binType);
    static bool CreateDataDirectory(const Path &executableDir);
    static bool CreateBehavioursLibrary(const Path &executableDir,
                                        BinType binType);
};
}

#endif  // GAMEBUILDER_H
