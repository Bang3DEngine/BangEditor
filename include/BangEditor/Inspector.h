#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Bang/IDestroyListener.h"
#include "Bang/IComponentListener.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIScrollPanel;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class UIVerticalLayout;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class InspectorWidget;

class Inspector : public EditorUITab,
                  public IDestroyListener,
                  public IComponentListener,
                  public IEditorListener
{
    GAMEOBJECT_EDITOR(Inspector);

public:
    Inspector();
    virtual ~Inspector();

    // GameObject
    void OnStart() override;
    void Update() override;

    GameObject *GetCurrentGameObject() const;

    // IDestroyListener
    void OnDestroyed(Object *destroyedObject) override;

    // IEditorListener
    void OnExplorerPathSelected(const Path &path) override;
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IComponentListener
    virtual void OnComponentAdded(Component *addedComponent, int index);
    virtual void OnComponentRemoved(Component *removedComponent);

private:
    List<InspectorWidget*> m_widgets;
    Map<Object*, InspectorWidget*> m_objToWidget;

    GameObject *p_currentGameObject = nullptr;
    UIVerticalLayout *p_mainVL   = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    UITextRenderer *p_titleText = nullptr;
    GameObject *p_titleSeparator = nullptr;

    GameObject* GetContainer() const;
    UIVerticalLayout *GetMainVL() const;
    UIScrollPanel* GetScrollPanel() const;

    void SetGameObject(GameObject *go);
    void AddWidget(InspectorWidget *widget, int index = -1);
    void RemoveWidget(InspectorWidget *widget);
    void RemoveWidget(int index);
    void Clear();
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTOR_H
