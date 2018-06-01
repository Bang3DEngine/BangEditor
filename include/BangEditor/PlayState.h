#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

enum class PlayState
{
    JUST_BEFORE_PLAYING,
    PLAYING,
    PAUSED,
    EDITING
};

NAMESPACE_BANG_EDITOR_END

#endif // PLAYSTATE_H
