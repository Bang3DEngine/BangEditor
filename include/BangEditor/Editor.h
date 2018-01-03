#ifndef EDITOR_H
#define EDITOR_H

#include "Bang/Bang.h"
#include "Bang/Object.h"
#include "Bang/GameObject.h"
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
FORWARD class TransformGizmo;

class IEditorSelectionListener : public virtual IEventListener
{
public:
    virtual void OnGameObjectSelected(GameObject *selectedGameObject) { }
    virtual void OnExplorerPathSelected(const Path &selectedPath) { }
};

class Editor : public EventEmitter<IEditorSelectionListener>,
               public IDestroyListener
{
public:
    static GameObject *GetSelectedGameObject();
    static void SelectGameObject(GameObject *selectedGameObject);

    template <class ListenerClass>
    static void RegisterListener(ListenerClass *selectionListener);

    template <class ListenerClass>
    static void UnRegisterListener(ListenerClass *selectionListener);

private:
    GameObject *p_selectedGameObject = nullptr;
    EditorSettings *m_editorSettings = nullptr;
    TransformGizmo *p_currentTransformGizmo = nullptr;

    Editor();
    virtual ~Editor();

    void Init();
    void _SelectGameObject(GameObject *selectedGameObject);

    static void OnPathSelected(const Path &path);

    EditorSettings* GetEditorSettings() const;
    static Editor* GetInstance();

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

    friend class Explorer;
    friend class EditorScene;
    friend class EditorSettings;
};

template <class ListenerClass>
void Editor::RegisterListener(ListenerClass *selectionListener)
{
    Editor::GetInstance()->EventEmitter<ListenerClass>::RegisterListener(selectionListener);
}

template <class ListenerClass>
void Editor::UnRegisterListener(ListenerClass *selectionListener)
{
    Editor::GetInstance()->EventEmitter<ListenerClass>::UnRegisterListener(selectionListener);
}

NAMESPACE_BANG_EDITOR_END

#endif // EDITOR_H

