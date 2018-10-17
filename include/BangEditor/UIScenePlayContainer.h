#ifndef UISCENEPLAYCONTAINER_H
#define UISCENEPLAYCONTAINER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/SceneManager.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

namespace Bang
{
class Camera;
class IEventsSceneManager;
class Path;
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class IEventsScenePlayer;

class UIScenePlayContainer : public UISceneContainer,
                             public EventListener<IEventsScenePlayer>,
                             public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(UIScenePlayContainer);

public:
    UIScenePlayContainer();
    virtual ~UIScenePlayContainer() override;

    // GameObject
    void Update() override;

private:
    Camera *GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderContainedScene(Scene *scene) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};
}

#endif  // UISCENEPLAYCONTAINER_H
