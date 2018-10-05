#include "BangEditor/UIContextMenu.h"

#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

#include "BangEditor/MenuItem.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIContextMenu::UIContextMenu()
{
    CONSTRUCT_CLASS_ID(UIContextMenu);
}

void UIContextMenu::ShowMenu()
{
    if (p_menu)
    {
        GameObject::Destroy(p_menu);
        p_menu = nullptr;
    }

    // if (!IsMenuBeingShown())
    {
        p_menu = GameObject::Create<ContextMenu>();
        if (m_createContextMenuCallback)
        {
            m_createContextMenuCallback(p_menu->GetRootItem());
        }

        if (p_menu->GetRootItem()->GetChildrenItems().IsEmpty())
        {
            GameObject::Destroy(p_menu);
        }
        else
        {
            p_menu->EventEmitter<IEventsDestroy>::RegisterListener(this);
            p_menu->SetParent( EditorSceneManager::GetEditorScene() );

            UICanvas::GetActive(this)->SetFocus(p_menu->GetFocusable());
        }
    }
}

bool UIContextMenu::IsMenuBeingShown() const
{
    return p_menu != nullptr;
}

void UIContextMenu::SetFocusable(UIFocusable *focusable)
{
    focusable->EventEmitter<IEventsDestroy>::RegisterListener(this);
    focusable->EventEmitter<IEventsFocus>::RegisterListener(this);
}

void UIContextMenu::SetCreateContextMenuCallback(
        UIContextMenu::CreateContextMenuCallback createCallback)
{
    m_createContextMenuCallback = createCallback;
}

UIEventResult UIContextMenu::OnUIEvent(UIFocusable*, const UIEvent &event)
{
    switch (event.type)
    {
        // case UIEvent::Type::FOCUS_LOST:
        //     GameObject::Destroy(p_menu);
        //     return UIEventResult::INTERCEPT;
        // break;

        case UIEvent::Type::MOUSE_CLICK_DOWN:
            if (event.mouse.button == MouseButton::RIGHT)
            {
                ShowMenu();
                return UIEventResult::INTERCEPT;
            }
        break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

void UIContextMenu::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (p_menu && object == p_menu)
    {
        p_menu = nullptr;
    }
}

// ContextMenu
ContextMenu::ContextMenu()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_rootItem = GameObject::Create<MenuItem>( MenuItem::MenuItemType::ROOT );

    RectTransform *rt = GetRootItem()->GetRectTransform();
    Vector2 mousePosNDC = Input::GetMousePositionNDC();
    rt->SetAnchors( mousePosNDC );
    rt->TranslateLocal( Vector3(0, 0, -0.001f) );

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    UIContentSizeFitter *csf = GetRootItem()->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::PREFERRED);
    csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

    GetRootItem()->AddComponent<UILayoutIgnorer>();
    GetRootItem()->EventEmitter<IEventsDestroy>::RegisterListener(this);
    GetRootItem()->SetDestroyOnClose(true);
    GetRootItem()->SetParent(this);

    m_justCreated = true;
}

UIEventResult ContextMenu::OnUIEvent(UIFocusable*, const UIEvent &event)
{
    switch (event.type)
    {
        // case UIEvent::Type::FOCUS_TAKEN:
        // break;
        //
        // case UIEvent::Type::FOCUS_LOST:
        //     GameObject::Destroy(this);
        //     return UIEventResult::INTERCEPT;
        // break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

void ContextMenu::Update()
{
    GameObject::Update();

    if (Input::GetMouseButtonDown(MouseButton::RIGHT) ||
        Input::GetMouseButtonDown(MouseButton::LEFT))
    {
        if (!m_justCreated &&
            !GetRootItem()->GetRectTransform()->IsMouseOver(true))
        {
            GameObject::Destroy(this);
        }
    }
    m_justCreated = false;
}

MenuItem *ContextMenu::GetRootItem() const
{
    return p_rootItem;
}

UIFocusable *ContextMenu::GetFocusable() const
{
    return p_focusable;
}

void ContextMenu::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    ASSERT(object == p_rootItem);
    GameObject::Destroy(this);
    GetFocusable()->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
    GetFocusable()->EventEmitter<IEventsFocus>::UnRegisterListener(this);
}
