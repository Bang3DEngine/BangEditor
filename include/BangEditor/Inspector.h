#ifndef INSPECTOR_H
#define INSPECTOR_H

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
                  public IEditorSelectionListener
{
    GAMEOBJECT_EDITOR(Inspector);

public:
    Inspector();
    virtual ~Inspector();

    // GameObject
    void OnStart() override;
    void Update() override;

    Object *GetCurrentObject() const;

    // IDestroyListener
    void OnDestroyed(Object *destroyedObject) override;

    // IEditorSelectionListener
    void OnExplorerPathSelected(const Path &path) override;
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

private:
    List<InspectorWidget*> m_widgets;

    Object *p_currentObject = nullptr;
    UIVerticalLayout *p_mainVL   = nullptr;
    UIScrollPanel *p_scrollPanel = nullptr;
    UITextRenderer *p_titleText = nullptr;
    GameObject *p_titleSeparator = nullptr;

    GameObject* GetContainer() const;
    UIVerticalLayout *GetMainVL() const;
    UIScrollPanel* GetScrollPanel() const;

    void SetGameObject(GameObject *go);
    void AddWidget(InspectorWidget *widget);
    void RemoveWidget(InspectorWidget *widget);
    void Clear();
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTOR_H
