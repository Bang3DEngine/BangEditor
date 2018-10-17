#ifndef HIERARCHYITEM_H
#define HIERARCHYITEM_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsName.h"
#include "Bang/ITreeItem.h"
#include "Bang/String.h"
#include "Bang/UIList.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsHierarchyItem.h"
#include "BangEditor/UIContextMenu.h"

namespace Bang
{
class IEventsName;
class Object;
class UIFocusable;
class UIDragDroppable;
class UILabel;
class UITextRenderer;
class UITree;
}

using namespace Bang;
namespace BangEditor
{
class IEventsHierarchyItem;
class MenuItem;
class UIContextMenu;

class HierarchyItem : public GameObject,
                      public ITreeItem,
                      public EventListener<IEventsName>,
                      public EventListener<IEventsFocus>,
                      public EventEmitter<IEventsHierarchyItem>
{
    GAMEOBJECT_EDITOR(HierarchyItem);

public:
    HierarchyItem();
    virtual ~HierarchyItem() override;

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

    // ITreeItem
    virtual UIFocusable *GetTreeItemFocusable() override;

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
}

#endif  // HIERARCHYITEM_H
