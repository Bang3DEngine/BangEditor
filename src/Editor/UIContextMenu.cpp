#include "BangEditor/UIContextMenu.h"

#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/IFocusable.h"
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

// UIContextMenu
void UIContextMenu::OnUpdate()
{
    Component::OnUpdate();

    if (Input::GetMouseButtonDown(MouseButton::RIGHT))
    {
        List<GameObject*> parts = m_parts;
        parts.PushBack(GetGameObject());

        for (GameObject *part : m_parts)
        {
            if (UICanvas::GetActive(this)->IsMouseOver(part, false))
            {
                ShowMenu();
                break;
            }
        }
    }
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
        p_menu->EventEmitter<IEventsDestroy>::RegisterListener(this);
        p_menu->SetParent( EditorSceneManager::GetEditorScene() );
    }

    if (p_menu->GetRootItem()->GetChildrenItems().IsEmpty())
    {
        GameObject::Destroy(p_menu);
    }
}

bool UIContextMenu::IsMenuBeingShown() const
{
    return p_menu != nullptr;
}

void UIContextMenu::AddButtonPart(GameObject *part)
{
    part->EventEmitter<IEventsDestroy>::RegisterListener(this);
    m_parts.PushBack(part);
}

void UIContextMenu::SetCreateContextMenuCallback(
        UIContextMenu::CreateContextMenuCallback createCallback)
{
    m_createContextMenuCallback = createCallback;
}

void UIContextMenu::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    m_parts.Remove( DCAST<GameObject*>(object) );
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
    GetRootItem()->SetDestroyOnClose(true);

    RectTransform *rt = GetRootItem()->GetRectTransform();
    Vector2 mousePosNDC = Input::GetMousePositionNDC();
    rt->SetAnchors( mousePosNDC );
    rt->TranslateLocal( Vector3(0, 0, -0.001f) );

    UIContentSizeFitter *csf = GetRootItem()->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::PREFERRED);
    csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

    GetRootItem()->AddComponent<UILayoutIgnorer>();
    GetRootItem()->EventEmitter<IEventsDestroy>::RegisterListener(this);
    GetRootItem()->SetDestroyOnClose(true);
    GetRootItem()->SetParent(this);

    m_justCreated = true;
}

void ContextMenu::Update()
{
    GameObject::Update();

    if (Input::GetMouseButtonDown(MouseButton::RIGHT) ||
        Input::GetMouseButtonDown(MouseButton::LEFT))
    {
        if (!m_justCreated && !GetRootItem()->GetRectTransform()->IsMouseOver(true))
        {
            GameObject::DestroyDelayed(this);
        }
    }
    m_justCreated = false;
}

MenuItem *ContextMenu::GetRootItem() const
{
    return p_rootItem;
}

void ContextMenu::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    ASSERT(object == p_rootItem);
    GameObject::DestroyDelayed(this);
}
