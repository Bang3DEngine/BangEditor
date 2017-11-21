#ifndef EDITOR_H
#define EDITOR_H

#include "Bang/Bang.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class IEditorSelectionListener : public virtual IEventListener
{
public:
    virtual void OnGameObjectSelected(GameObject *selectedGameObject) = 0;
};




class Editor : public EventEmitter<IEditorSelectionListener>
{
public:
    static void SelectGameObject(GameObject *selectedGameObject);

    template <class ListenerClass>
    static void RegisterListener(ListenerClass *selectionListener);

    template <class ListenerClass>
    static void UnRegisterListener(ListenerClass *selectionListener);

private:
    Editor() = default;
    virtual ~Editor() = default;

    GameObject *p_selectedGameObject = nullptr;
    void _EmitGameObjectSelected(GameObject *selectedGameObject);

    static Editor* GetInstance();

    friend class EditorScene;
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

