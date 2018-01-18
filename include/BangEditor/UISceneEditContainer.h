#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/SceneManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/UISceneContainer.h"

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

private:
    Camera* GetSceneCamera(Scene *scene) override;

    // IEditorListener
    void OnPlayStateChanged(EditorPlayState previousPlayState,
                            EditorPlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneOpen(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEEDITCONTAINER_H

