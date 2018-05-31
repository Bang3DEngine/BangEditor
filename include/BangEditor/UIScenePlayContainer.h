#ifndef UISCENEPLAYCONTAINER_H
#define UISCENEPLAYCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/SceneManager.h"
#include "Bang/EventListener.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIScenePlayContainer : public UISceneContainer,
                             public EventListener<IScenePlayerListener>,
                             public EventListener<ISceneManagerListener>
{
    GAMEOBJECT_EDITOR(UIScenePlayContainer);

public:
	UIScenePlayContainer();
	virtual ~UIScenePlayContainer();

    // GameObject
    void Update() override;

private:
    Camera* GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderScene(Scene *scene) override;

    // IScenePlayerListener
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEPLAYCONTAINER_H

