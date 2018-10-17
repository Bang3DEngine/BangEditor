#ifndef IEVENTSSCENEPLAYER_H
#define IEVENTSSCENEPLAYER_H

#include "Bang/IEvents.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/PlayState.h"

namespace BangEditor
{
class IEventsScenePlayer
{
    IEVENTS_NS(IEventsScenePlayer);

public:
    virtual void OnPlayStateChanged(PlayState previousPlayState,
                                    PlayState newPlayState)
    {
        BANG_UNUSED_2(previousPlayState, newPlayState);
    }
};
}

#endif  // IEVENTSSCENEPLAYER_H
