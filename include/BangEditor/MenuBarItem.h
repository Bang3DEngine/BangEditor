#ifndef MENUBARITEM_H
#define MENUBARITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/UIButton.h"
#include "Bang/GameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIButton;
FORWARD class UIVerticalLayout;
FORWARD class UITintedButton;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBarItem : public GameObject,
                    public IUIButtonListener
{
    GAMEOBJECT(MenuBarItem)

public:
    void AddSeparator();
    void SetAsChild(MenuBarItem *childItem);
    MenuBarItem* SetAsChild(const String &text);

    UITextRenderer *GetText() const;
    UIButton *GetButton() const;

protected:
    MenuBarItem() = default;
    MenuBarItem(bool topItem);
    virtual ~MenuBarItem();

private:
    bool m_isTopItem = false;
    UITextRenderer *m_text = nullptr;
    UITintedButton *m_buttonWithTint = nullptr;

    List<MenuBarItem*> m_childrenItems;
    GameObject *m_childrenContainer = nullptr;
    UIVerticalLayout *m_childrenContainerVL = nullptr;

    virtual void OnButton_MouseEnter(UIButton *btn);
    virtual void OnButton_MouseExit(UIButton *btn);
    virtual void OnButton_Clicked(UIButton *btn);

    friend class Bang::ObjectManager;
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBARITEM_H
