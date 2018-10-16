#ifndef UICONTEXTMENU_H
#define UICONTEXTMENU_H

#include <functional>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/Component.h"
#include "Bang/ComponentMacros.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/EditorComponentsClassIds.h"
#include "BangEditor/MenuItem.h"

namespace BangEditor {
class MenuItem;
}  // namespace BangEditor
namespace Bang {
class IEventsDestroy;
class UIFocusable;
template <class T> class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ContextMenu;

class UIContextMenu : public Component,
                      public EventListener<IEventsFocus>,
                      public EventListener<IEventsDestroy>
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(UIContextMenu)

public:
    UIContextMenu();
    virtual ~UIContextMenu() = default;

    void ShowMenu();
    bool IsMenuBeingShown() const;
    void SetFocusable(UIFocusable *focusable);

    using CreateContextMenuCallback = std::function<void(MenuItem *menuRootItem)>;
    void SetCreateContextMenuCallback(
                std::function<void(MenuItem*)> createCallback);

private:
    ContextMenu *p_menu = nullptr;

    CreateContextMenuCallback m_createContextMenuCallback;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};


// ContextMenu
class ContextMenu : public GameObject,
                    public EventListener<IEventsFocus>
{
    GAMEOBJECT_EDITOR(ContextMenu);

public:

    // GameObject
    void Update() override;

    MenuItem *GetRootItem() const;
    UIFocusable *GetFocusable() const;

private:
    bool m_justCreated = false;
    MenuItem *p_rootItem = nullptr;
    UIFocusable *p_focusable = nullptr;

    ContextMenu();
    virtual ~ContextMenu() = default;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UICONTEXTMENU_H

