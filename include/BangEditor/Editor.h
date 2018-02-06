#ifndef EDITOR_H
#define EDITOR_H

#include "Bang/Bang.h"
#include "Bang/Object.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"
#include "Bang/IDestroyListener.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Scene;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class EditorSettings;

class IEditorListener : public virtual IEventListener
{
    EVENTLISTENER(IEditorListener)

public:
    virtual void OnGameObjectSelected(GameObject *selectedGameObject) { }
    virtual void OnExplorerPathSelected(const Path &selectedPath) { }
};

class Editor : public EventEmitter<IEditorListener>,
               public ISceneManagerListener,
               public IDestroyListener
{
public:
    static GameObject *GetSelectedGameObject();
    static void SelectGameObject(GameObject *selectedGameObject);

    static bool IsEditingScene();

    static Editor* GetInstance();

private:
    GameObject *p_selectedGameObject = nullptr;
    EditorSettings *m_editorSettings = nullptr;

    Editor();
    virtual ~Editor();

    void Init();
    void _SelectGameObject(GameObject *selectedGameObject);

    static void OnPathSelected(const Path &path);

    EditorSettings* GetEditorSettings() const;

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    friend class Explorer;
    friend class EditorScene;
    friend class EditorSettings;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITOR_H

