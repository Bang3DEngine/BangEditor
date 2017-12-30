#ifndef UICONTEXTMENU_H
#define UICONTEXTMENU_H

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

class UIContextMenu : public Component,
                      public IDestroyListener
{
    COMPONENT(UIContextMenu)

public:
    UIContextMenu() = default;
    virtual ~UIContextMenu() = default;

    // Component
    void OnUpdate() override;

    void ShowMenu();
    bool IsMenuBeingShown() const;
    void AddButtonPart(GameObject *part);

    using CreateContextMenuCallback = std::function<void(MenuItem *menuRootItem)>;
    void SetCreateContextMenuCallback(CreateContextMenuCallback createCallback);

private:
    List<GameObject*> m_parts;
    Menu *p_menu = nullptr;

    CreateContextMenuCallback m_createContextMenuCallback;

    // IDestroyListener
    void OnDestroyed(Object *object) override;
};


// Menu
class Menu : public GameObject
{
    GAMEOBJECT_EDITOR(Menu);

public:

    // GameObject
    void Update() override;

    MenuItem *GetRootItem() const;

private:
    MenuItem *p_rootItem = nullptr;
    bool m_justCreated = false;

    Menu();
    virtual ~Menu() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // UICONTEXTMENU_H

