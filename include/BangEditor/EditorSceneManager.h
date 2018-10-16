#ifndef EDITORSCENEMANAGER_H
#define EDITORSCENEMANAGER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/SceneManager.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class BehaviourManager;
class IEventsSceneManager;
class Path;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorBehaviourManager;
FORWARD class EditorScene;

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

    Scene *GetOpenScene_() const;
    EditorScene *GetEditorScene_() const;

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
