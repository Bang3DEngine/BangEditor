#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/SceneManager.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneEditContainer : public UISceneContainer,
                             public IScenePlayerListener,
                             public ISceneManagerListener
{
    GAMEOBJECT_EDITOR(UISceneEditContainer);

public:
	UISceneEditContainer();
	virtual ~UISceneEditContainer();

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    void HandleSelection();

private:
    UIImageRenderer *p_cameraPreviewImg = nullptr;

    void RenderCameraPreviewIfSelected();
    Camera* GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderScene(Scene *scene) override;
    void OnRenderNeededSceneFinished() override;

    // IScenePlayerListener
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEEDITCONTAINER_H

