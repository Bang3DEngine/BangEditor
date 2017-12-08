#ifndef MENUITEM_H
#define MENUITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class IFocusable;
FORWARD class UIFocusable;
FORWARD class UIVerticalLayout;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuItem : public GameObject
{
    GAMEOBJECT(MenuItem)

public:
    enum class MenuItemType { Root, Top, Normal };

    // GameObject
    void Update() override;

    void AddSeparator();
    void AddItem(MenuItem *childItem);
    MenuItem* AddItem(const String &text);

    UITextRenderer *GetText() const;
    UIFocusable *GetButton() const;

    bool MustDisplayChildren() const;
    void SetFontSize(uint fontSize);

protected:
    MenuItem(MenuItemType itemType = MenuItemType::Normal);
    virtual ~MenuItem();

private:
    MenuItemType m_itemType = Undef<MenuItemType>();
    uint m_fontSize = Undef<uint>();

    UIFocusable *m_button = nullptr;
    UITextRenderer *m_text = nullptr;

    List<MenuItem*> m_childrenItems;
    GameObject *m_childrenContainer = nullptr;
    UIVerticalLayout *m_childrenContainerVL = nullptr;

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUITEM_H
