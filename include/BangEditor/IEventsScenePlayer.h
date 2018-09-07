#ifndef IEVENTSSCENEPLAYER_H
#define IEVENTSSCENEPLAYER_H

#include "Bang/IEvents.h"

#include "BangEditor/PlayState.h"
#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_EDITOR_BEGIN

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


NAMESPACE_BANG_EDITOR_END

#endif // IEVENTSSCENEPLAYER_H
