#ifndef EDITORSCENEMANAGER_H
#define EDITORSCENEMANAGER_H

#include "Bang/SceneManager.h"
#include "Bang/EventListener.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;
FORWARD class EditorBehaviourManager;

class EditorSceneManager : public SceneManager,
                           public EventListener<IEventsSceneManager>
{
public:
    EditorSceneManager();
    virtual ~EditorSceneManager();

    static Scene *GetOpenScene();
    static EditorScene *GetEditorScene();
    static EditorSceneManager *GetActive();
    EditorBehaviourManager *GetEditorBehaviourManager() const;

protected:
    EditorScene *p_editorScene = nullptr;

    Scene *_GetOpenScene() const;
    EditorScene *_GetEditorScene() const;

    static void SetActiveScene(Scene *activeScene);

private:
    BehaviourManager *CreateBehaviourManager() const override;
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    friend class EditorScene;
    friend class ScenePlayer;
    friend class EditorApplication;

};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENEMANAGER_H
