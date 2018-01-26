#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include "Bang/UIList.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"

#include "BangEditor/UIContextMenu.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITree;
FORWARD class UILabel;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class HierarchyItem;

class IHierarchyItemListener : public IEventListener
{
public:
    virtual void OnCreateEmpty(HierarchyItem *item) = 0;
    virtual void OnRename(HierarchyItem *item) = 0;
    virtual void OnRemove(HierarchyItem *item) = 0;
    virtual void OnCopy(HierarchyItem *item) = 0;
    virtual void OnCut(HierarchyItem *item) = 0;
    virtual void OnPaste(HierarchyItem *item) = 0;
    virtual void OnDuplicate(HierarchyItem *item) = 0;
    virtual void OnCreatePrefab(HierarchyItem *item) = 0;
};

class HierarchyItem : public GameObject,
                      public INameListener,
                      public EventEmitter<IHierarchyItemListener>
{
    GAMEOBJECT_EDITOR(HierarchyItem);

public:
    HierarchyItem();
    virtual ~HierarchyItem();

    // GameObject
    void OnStart() override;
    void Update() override;

    void SetReferencedGameObject(GameObject *referencedGameObject);
    GameObject *GetReferencedGameObject() const;

    void CreateEmpty();
    void Rename();
    void Remove();
    void Copy();
    void Cut();
    void Paste();
    void Duplicate();
    void CreatePrefab();

    // INameListener
    void OnNameChanged(GameObject *go, const String &oldName,
                       const String &newName) override;

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem);

    // UIList Item
    void OnSelectionCallback(UIList::Action action);

    String ToString() const override;

private:
    String m_text = "";
    GameObject *p_refGameObject = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UITextRenderer *p_textRenderer = nullptr;

    void SetText(const String &text);
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHYITEM_H

