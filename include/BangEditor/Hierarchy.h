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
                  public EventListener<IEventsDestroy>,
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

    // Object
    void Update() override;

    // IEventsChildren
    void OnChildAdded(GameObject *parent, GameObject *addedChild) override;
    void OnChildRemoved(GameObject *parent, GameObject *removedChild) override;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEventsHierarchyItem
    virtual void OnCreateEmpty(HierarchyItem *item) override;
    virtual void OnRename(HierarchyItem *item) override;
    virtual void OnRemove(HierarchyItem *item) override;
    virtual void OnCopy(HierarchyItem *item) override;
    virtual void OnCut(HierarchyItem *item) override;
    virtual void OnPaste(HierarchyItem *item) override;
    virtual void OnDuplicate(HierarchyItem *item) override;
    virtual void OnCreatePrefab(HierarchyItem *item) override;

    // IUITreeListener
    virtual void OnItemMoved(GameObject *item,
                             GameObject *oldParentItem, int oldIndexInsideParent,
                             GameObject *newParentItem, int newIndexInsideParent)
                             override;

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

    GOItem *GetSelectedGameObject() const;
    GOItem *GetSelectedItem() const;

    HierarchyItem* GetItemFromGameObject(GameObject *go) const;
    GameObject* GetGameObjectFromItem(GOItem *item) const;

    static void OnShortcutPressed(const Shortcut &shortcut);

    UITree *GetUITree() const;

    friend class HierarchyItem;
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHY_H
