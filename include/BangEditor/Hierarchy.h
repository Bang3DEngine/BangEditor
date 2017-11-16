#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "Bang/Map.h"
#include "Bang/UIList.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorUITab.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITree;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class HierarchyItem;

class Hierarchy : public EditorUITab,
                  public IEditorSelectionListener
{
    GAMEOBJECT_EDITOR(Hierarchy)

public:
    Hierarchy();
    virtual ~Hierarchy();

    void ToggleItemCollapsed(HierarchyItem *item);
    void SetItemCollapsed(HierarchyItem *item, bool collapsed);
    void Clear();

    // Object
    void OnStart() override;

    // IEditorSelectionListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    static Hierarchy *GetInstance();

private:
    UITree *p_tree = nullptr;
    Map<GameObject*, HierarchyItem*> m_gameObjectToItem;

    void UpdateFromScene();
    void UpdateItemFromGameObject(GOItem *item, GameObject *gameObject);
    void TreeSelectionCallback(GOItem *item, UIList::Action action);
    void AddGameObjectItemToHierarchy(GameObject *go, bool topItem);

    HierarchyItem* GetItemFromGameObject(GameObject *go);

    UITree *GetUITree() const;

    friend class HierarchyItem;
};
NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHY_H
