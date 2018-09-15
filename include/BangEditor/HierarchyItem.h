#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include "Bang/Bang.h"
#include "Bang/UIList.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsFocus.h"

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
                      public EventListener<IEventsFocus>,
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

    void Rename();
    void Remove();
    void Copy();
    void Cut();
    void Paste();
    void Duplicate();
    void CreatePrefab();

    void UpdateEnabledDisabledColor();

    UIFocusable *GetFocusable() const;
    GameObject *GetReferencedGameObject() const;

    // IEventsObject
    void OnEnabled(Object *obj) override;
    void OnDisabled(Object *obj) override;

    // IEventsName
    void OnNameChanged(GameObject *go,
                       const String &oldName,
                       const String &newName) override;

    // UIContextMenu callback
    void OnCreateContextMenu(MenuItem *menuRootItem);

    // UIList Item
    void OnSelectionCallback(UIList::Action action);

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    String ToString() const override;

private:
    UIFocusable *p_focusable = nullptr;
    GameObject *p_refGameObject = nullptr;
    UIContextMenu *p_contextMenu = nullptr;
    UITextRenderer *p_textRenderer = nullptr;

    void SetText(const String &text);
};

NAMESPACE_BANG_EDITOR_END

#endif // HIERARCHYITEM_H

