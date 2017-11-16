#ifndef UICONTEXTMENU_H
#define UICONTEXTMENU_H

#include "Bang/UIButton.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

#include "BangEditor/MenuItem.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class Menu;

class IUIContextMenuable : public IEventListener
{
public:
    virtual void OnSetContextMenu(Menu *menu) = 0;
};

class UIContextMenu : public Component,
                      public EventEmitter<IUIContextMenuable>
{
    COMPONENT(UIContextMenu);

public:
    UIContextMenu() = default;
    virtual ~UIContextMenu() = default;

    // Component
    void OnUpdate() override;

    void AddButtonPart(GameObject *part);

private:
    List<GameObject*> m_parts;
};


// Menu
class Menu : public GameObject
{
    GAMEOBJECT_EDITOR(Menu);

public:

    // GameObject
    void Update() override;

    MenuItem* AddItem(const String &text = "MenuItem");

private:
    Menu();
    virtual ~Menu() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // UICONTEXTMENU_H

