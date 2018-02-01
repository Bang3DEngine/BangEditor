#ifndef UISCENEPLAYCONTAINER_H
#define UISCENEPLAYCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/SceneManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/UISceneContainer.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIScenePlayContainer : public UISceneContainer,
                             public IEditorListener,
                             public ISceneManagerListener
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

    // IEditorListener
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEPLAYCONTAINER_H

