#ifndef GAMEBUILDER_H
#define GAMEBUILDER_H

#include "Bang/Path.h"
#include "Bang/BinType.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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
    static bool     CompileGameExecutable(BinType binType);
    static bool     CreateDataDirectory(const Path &executableDir);
    static bool     CreateBehavioursLibrary(const Path &executableDir,
                                      BinType binType);
};

NAMESPACE_BANG_EDITOR_END

#endif // GAMEBUILDER_H
