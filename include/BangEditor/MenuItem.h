#ifndef MENUITEM_H
#define MENUITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/UIList.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIList;
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuItem : public GameObject
{
    GAMEOBJECT(MenuItem);

public:
    enum class MenuItemType { Root, Top, Normal };

    // GameObject
    void Update() override;

    void AddSeparator();
    void AddItem(MenuItem *childItem);
    MenuItem* AddItem(const String &text);

    UIList *GetChildrenList() const;
    UITextRenderer *GetText() const;
    UIFocusable *GetFocusable() const;

    bool MustDisplayChildren() const;
    void SetFontSize(uint fontSize);
    void SetDestroyOnClose(bool destroyOnSelect);

    using ItemSelectedCallback = std::function<void(MenuItem *selectedItem)>;
    void SetSelectedCallback(ItemSelectedCallback selectedCallback);

protected:
    MenuItem(MenuItemType itemType = MenuItemType::Normal);
    virtual ~MenuItem();

private:
    ItemSelectedCallback m_selectedCallback;
    bool m_destroyOnClose = false;

    MenuItemType m_itemType = Undef<MenuItemType>();
    uint m_fontSize = Undef<uint>();

    UIFocusable *p_focusable = nullptr;
    UITextRenderer *p_text = nullptr;

    List<MenuItem*> p_childrenItems;
    MenuItem* p_parentItem = nullptr;
    UIList *p_childrenList = nullptr;
    UIImageRenderer *p_topBg = nullptr;
    UIImageRenderer *p_rightArrow = nullptr;

    void CloseRecursiveUp();
    bool IsSelectedInList() const;
    static void OnListSelectionCallback(GameObject *item, UIList::Action action);

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUITEM_H
