#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "Bang/Map.h"
#include "Bang/UIList.h"
#include "Bang/ICreateListener.h"
#include "Bang/IChildrenListener.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorUITab.h"
#include "BangEditor/ShortcutManager.h"
#include "BangEditor/IEditorOpenSceneListener.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITree;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class HierarchyItem;

class Hierarchy : public EditorUITab,
                  public ICreateListener,
                  public IEditorListener,
                  public IEditorOpenSceneListener
{
    GAMEOBJECT_EDITOR(Hierarchy);

public:
    Hierarchy();
    virtual ~Hierarchy();

    void Clear();

    bool IsItemCollapsed(HierarchyItem *item) const;

    // Object
    void Update() override;

    // ICreateListener
    void OnCreated(Object *object) override;

    // IChildrenListener
    void OnChildAdded(GameObject *parent, GameObject *addedChild) override;
    void OnChildRemoved(GameObject *parent, GameObject *removedChild) override;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEditorOpenSceneListener
    virtual void OnOpenScene(Scene *scene) override;

    static Hierarchy *GetInstance();

private:
    UITree *p_tree = nullptr;
    Map<GameObject*, HierarchyItem*> m_gameObjectToItem;

    void TreeSelectionCallback(GOItem *item, UIList::Action action);
    void AddGameObject(GameObject *go);
    void RemoveGameObject(GameObject *go);
    void OnCreatedDestroyed(Object *object, bool created);

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
