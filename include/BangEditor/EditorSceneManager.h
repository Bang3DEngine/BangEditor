#ifndef EDITORSCENEMANAGER_H
#define EDITORSCENEMANAGER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/SceneManager.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class BehaviourManager;
class IEventsSceneManager;
class Path;
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class EditorBehaviourManager;
class EditorScene;

class EditorSceneManager : public SceneManager,
                           public EventListener<IEventsSceneManager>
{
public:
    EditorSceneManager();
    virtual ~EditorSceneManager() override;

    static Scene *GetOpenScene();
    static EditorScene *GetEditorScene();
    static EditorSceneManager *GetActive();
    EditorBehaviourManager *GetEditorBehaviourManager() const;

protected:
    EditorScene *p_editorScene = nullptr;

    Scene *GetOpenScene_() const;
    EditorScene *GetEditorScene_() const;
    Scene *GetObjectPtrLookupScene_() const override;

    static void SetActiveScene(Scene *activeScene);

private:
    BehaviourManager *CreateBehaviourManager() const override;
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    friend class EditorScene;
    friend class ScenePlayer;
    friend class EditorApplication;
};
}

#endif  // EDITORSCENEMANAGER_H
