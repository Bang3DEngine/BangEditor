#ifndef EDITOR_H
#define EDITOR_H

#include "Bang/Bang.h"
#include "Bang/Object.h"
#include "Bang/GameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Scene;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class Editor : public EventEmitter<IEventsEditor>,
               public EventListener<IEventsSceneManager>,
               public EventListener<IEventsDestroy>
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
    void SelectGameObject_(GameObject *selectedGameObject);

    static void OnPathSelected(const Path &path);

    EditorSettings* GetEditorSettings() const;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    friend class Explorer;
    friend class EditorSettings;
    friend class EditorApplication;
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITOR_H

