#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/SceneManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/UISceneContainer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneEditContainer : public UISceneContainer,
                             public IEditorListener,
                             public ISceneManagerListener
{
    GAMEOBJECT_EDITOR(UISceneEditContainer);

public:
	UISceneEditContainer();
	virtual ~UISceneEditContainer();

    // GameObject
    void Update() override;

    void HandleSelection();

private:
    UIImageRenderer *p_cameraPreviewImg = nullptr;

    Camera* GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderScene(Scene *scene) override;

    // IEditorListener
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneOpen(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEEDITCONTAINER_H

