#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "BangEditor/BangEditor.h"

namespace BangEditor
{
enum class PlayState
{
    JUST_BEFORE_PLAYING,
    PLAYING,
    PAUSED,
    EDITING
};
}

#endif  // PLAYSTATE_H
