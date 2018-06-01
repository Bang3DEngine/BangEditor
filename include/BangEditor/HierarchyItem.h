#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include "Bang/Bang.h"
#include "Bang/UIList.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/IEventsHierarchyItem.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITree;
FORWARD class UILabel;
FORWARD class UIDragDroppable;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class HierarchyItem : public GameObject,
                      public EventListener<IEventsName>,
                      public EventEmitter<IEventsHierarchyItem>
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

    // IEventsName
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

