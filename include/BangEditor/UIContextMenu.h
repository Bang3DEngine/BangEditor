#ifndef UICONTEXTMENU_H
#define UICONTEXTMENU_H

#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"

#include "BangEditor/MenuItem.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ContextMenu;

class UIContextMenu : public Component,
                      public EventListener<IDestroyListener>
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
    ContextMenu *p_menu = nullptr;

    CreateContextMenuCallback m_createContextMenuCallback;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;
};


// ContextMenu
class ContextMenu : public GameObject,
                    public EventListener<IDestroyListener>
{
    GAMEOBJECT_EDITOR(ContextMenu);

public:

    // GameObject
    void Update() override;

    MenuItem *GetRootItem() const;

    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

private:
    MenuItem *p_rootItem = nullptr;
    bool m_justCreated = false;

    ContextMenu();
    virtual ~ContextMenu() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // UICONTEXTMENU_H

