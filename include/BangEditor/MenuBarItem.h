#ifndef MENUBARITEM_H
#define MENUBARITEM_H

#include "BangEditor/BangEditor.h"

#include "Bang/List.h"
#include "Bang/UIInteractive.h"
#include "Bang/UIGameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIVerticalLayout;
FORWARD class UITintedInteractive;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MenuBarItem : public UIGameObject,
                    public UIInteractiveListener
{
public:
    MenuBarItem(bool toTheRight);
    virtual ~MenuBarItem();

    void Update() override;

    void AddChild(MenuBarItem *childItem);
    MenuBarItem* AddChild(const String &text);

    UITextRenderer *GetText() const;

private:
    UITextRenderer *m_text = nullptr;
    UITintedInteractive *m_buttonWithTint = nullptr;

    List<MenuBarItem*> m_childrenItems;
    UIGameObject *m_childrenContainer = nullptr;
    UIVerticalLayout *m_childrenContainerVL = nullptr;

    virtual void OnButton_MouseEnter(UIInteractive *btn);
    virtual void OnButton_MouseExit(UIInteractive *btn);
    virtual void OnButton_Clicked(UIInteractive *btn);
};

NAMESPACE_BANG_EDITOR_END

#endif // MENUBARITEM_H
