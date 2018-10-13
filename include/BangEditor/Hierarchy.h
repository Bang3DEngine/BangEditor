#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "Bang/UMap.h"
#include "Bang/UIList.h"
#include "Bang/SceneManager.h"
#include "Bang/IEventsUITree.h"
#include "Bang/IEventsChildren.h"

#include "BangEditor/Editor.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/ShortcutManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITree;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class HierarchyItem;

class Hierarchy : public GameObject,
                  public EventListener<IEventsEditor>,
                  public EventListener<IEventsFocus>,
                  public EventListener<IEventsSceneManager>,
                  public EventListener<IEventsHierarchyItem>,
                  public EventListener<IEventsUITree>
{
    GAMEOBJECT_EDITOR(Hierarchy);

public:
    Hierarchy();
    virtual ~Hierarchy();

    void Clear();

    bool IsItemCollapsed(HierarchyItem *item) const;

    HierarchyItem* GetItemFromGameObject(GameObject *go) const;
    GameObject* GetGameObjectFromItem(GOItem *item) const;

    // Object
    void Update() override;

    // IEventsChildren
    void OnChildAdded(GameObject *parent, GameObject *addedChild) override;
    void OnChildRemoved(GameObject *parent, GameObject *removedChild) override;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;
    void DuplicateSelectedItem();

    // IEventsHierarchyItem
    virtual void OnRename(HierarchyItem *item) override;
    virtual void OnRemove(HierarchyItem *item) override;
    virtual void OnCopy(HierarchyItem *item) override;
    virtual void OnCut(HierarchyItem *item) override;
    virtual void OnPaste(HierarchyItem *item) override;
    virtual void OnDuplicate(HierarchyItem *item) override;
    virtual void OnCreatePrefab(HierarchyItem *item) override;

    // IEventsUITree
    virtual void OnItemMoved(GOItem *item,
                             GOItem *oldParentItem,
                             int oldIndexInsideParent,
                             GOItem *newParentItem,
                             int newIndexInsideParent) override;
    virtual void OnDropOutside(UIDragDroppable *dropped) override;
    virtual void OnDropFromOutside(UIDragDroppable *dropped,
                                   GameObject *newParentItem,
                                   int newIndexInsideParent) override;
    virtual bool AcceptDragOrDrop(UIDragDroppable *dd) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // UIContextMenu
    void OnCreateContextMenu(MenuItem *menuRootItem);

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    static Hierarchy *GetInstance();

private:
    UITree *p_tree = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UMap<GameObject*, HierarchyItem*> m_gameObjectToItem;

    void TreeSelectionCallback(GOItem *item, UIList::Action action);
    void AddGameObject(GameObject *go);
    void RemoveGameObject(GameObject *go);
    void RemoveGameObjectItem(HierarchyItem *item);

    GOItem *GetSelectedGameObject() const;
    GOItem *GetSelectedItem() const;

    UITree *GetUITree() const;

    friend class HierarchyItem;
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHY_H
