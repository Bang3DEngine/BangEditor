#ifndef UISCENEPLAYCONTAINER_H
#define UISCENEPLAYCONTAINER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/SceneManager.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

namespace BangEditor {
class IEventsScenePlayer;
}  // namespace BangEditor
namespace Bang {
class Camera;
class IEventsSceneManager;
class Path;
class Scene;
}  // namespace Bang

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIScenePlayContainer : public UISceneContainer,
                             public EventListener<IEventsScenePlayer>,
                             public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(UIScenePlayContainer);

public:
	UIScenePlayContainer();
	virtual ~UIScenePlayContainer();

    // GameObject
    void Update() override;

private:
    Camera* GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderContainedScene(Scene *scene) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEPLAYCONTAINER_H

