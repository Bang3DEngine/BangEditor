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

class EditorSceneManager : public SceneManager
{
public:
    EditorSceneManager() = default;
    virtual ~EditorSceneManager() = default;

    static Scene *GetOpenScene();
    static EditorScene *GetEditorScene();

protected:
    Scene *_GetOpenScene() const;
    EditorScene *_GetEditorScene() const;

    void _Update() override;
    void _LoadScene(Scene *scene) override;

    static void SetActiveScene(Scene *activeScene);

    static EditorSceneManager *GetInstance();

    friend class EditorScene;
    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORSCENEMANAGER_H
