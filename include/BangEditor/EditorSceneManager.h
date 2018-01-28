#ifndef EDITORSCENEMANAGER_H
#define EDITORSCENEMANAGER_H

#include "BangEditor/BangEditor.h"

#include "Bang/SceneManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorScene;
FORWARD class EditorBehaviourManager;

class EditorSceneManager : public SceneManager
{
public:
    EditorSceneManager() = default;
    virtual ~EditorSceneManager();

    static Scene *GetOpenScene();
    static Path GetOpenScenePath();
    static EditorScene *GetEditorScene();

    static EditorSceneManager *GetInstance();

    EditorBehaviourManager *GetEditorBehaviourManager() const;

protected:
    EditorScene *p_editorScene = nullptr;

    Scene *_GetOpenScene() const;
    EditorScene *_GetEditorScene() const;

    static void SetActiveScene(Scene *activeScene);

private:
    void _Update() override;
    void _LoadSceneInstantly(Scene *scene) override;

    BehaviourManager *CreateBehaviourManager() const override;

    friend class EditorScene;
    friend class ScenePlayer;
    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENEMANAGER_H
