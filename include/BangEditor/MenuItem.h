#ifndef MENUITEM_H
#define MENUITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/UIList.h"
#include "Bang/GameObject.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIList;
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuItem : public GameObject
{
    GAMEOBJECT(MenuItem);

public:
    enum class MenuItemType
    {
        ROOT,
        TOP,
        NORMAL,
        SEPARATOR
    };

    // GameObject
    void Update() override;

    void AddSeparator();
    void AddItem(MenuItem *childItem);
    MenuItem* AddItem(const String &text);

    void Close(bool recursiveUp);
    void SetForceShow(bool forceShow);
    void SetOverAndActionEnabled(bool enabled);
    void SetOverAndActionEnabledRecursively(bool enabled);
    void SetDropDownEnabled(bool enabled);
    void SetDropDownEnabledRecursively(bool enabled);

    bool IsOverAndActionEnabled() const;
    bool IsDropDownEnabled() const;
    UIList *GetChildrenList() const;
    UITextRenderer *GetText() const;
    UIFocusable *GetFocusable() const;
    MenuItemType GetItemType() const;
    bool GetDestroyOnClose() const;
    float GetFontSize() const;
    bool IsForcedShow() const;
    MenuItem *GetParentItem() const;
    const List<MenuItem*>& GetChildrenItems() const;

    bool MustDisplayChildren() const;
    void SetFontSize(uint fontSize);
    void SetDestroyOnClose(bool destroyOnSelect);

    using ItemSelectedCallback = std::function<void(MenuItem *selectedItem)>;
    void SetSelectedCallback(ItemSelectedCallback selectedCallback);

protected:
    MenuItem(MenuItemType itemType = MenuItemType::NORMAL);
    virtual ~MenuItem();

private:
    ItemSelectedCallback m_selectedCallback;
    bool m_destroyOnClose = false;
    bool m_forcedShow = false;

    bool m_overAndActionEnabled = true;
    bool m_dropDownEnabled = true;

    MenuItemType m_itemType = Undef<MenuItemType>();
    uint m_fontSize = Undef<uint>();

    UIFocusable *p_focusable = nullptr;
    UITextRenderer *p_text = nullptr;

    List<MenuItem*> p_childrenItems;
    MenuItem* p_parentItem = nullptr;
    UIList *p_childrenList = nullptr;
    UIImageRenderer *p_topBg = nullptr;
    UIImageRenderer *p_rightArrow = nullptr;

    MenuItem *GetTopOrRootItem() const;
    bool IsSelected() const;
    void AdjustToBeInsideScreen();
    static void OnListSelectionCallback(GameObject *item, UIList::Action action);
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUITEM_H
