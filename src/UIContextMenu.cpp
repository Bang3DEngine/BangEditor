#include "BangEditor/UIContextMenu.h"

#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
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

    if (Input::GetMouseButtonDown(MouseButton::Right))
    {
        List<GameObject*> parts = m_parts;
        parts.PushBack(GetGameObject());

        for (GameObject *part : m_parts)
        {
            if (UICanvas::GetActive(this)->IsMouseOver(part, true))
            {
                ShowMenu();
                break;
            }
        }
    }
}

void UIContextMenu::ShowMenu()
{
    if (!IsMenuBeingShown())
    {
        p_menu = GameObject::Create<Menu>();
        if (m_createContextMenuCallback)
        {
            m_createContextMenuCallback(p_menu->GetRootItem());
        }
        p_menu->EventEmitter<IDestroyListener>::RegisterListener(this);
        p_menu->SetParent( EditorSceneManager::GetEditorScene() );
    }
}

bool UIContextMenu::IsMenuBeingShown() const
{
    return p_menu != nullptr;
}

void UIContextMenu::AddButtonPart(GameObject *part)
{
    m_parts.PushBack(part);
}

void UIContextMenu::SetCreateContextMenuCallback(
        UIContextMenu::CreateContextMenuCallback createCallback)
{
    m_createContextMenuCallback = createCallback;
}

void UIContextMenu::OnDestroyed(Object *object)
{
    ASSERT(p_menu && object == p_menu);
    p_menu = nullptr;
}

// Menu
Menu::Menu()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_rootItem = GameObject::Create<MenuItem>( MenuItem::MenuItemType::Root );
    p_rootItem->GetChildrenList()->SetIdleColor( Color::LightGray );
    p_rootItem->SetDestroyOnClose(true);

    RectTransform *rt = p_rootItem->GetRectTransform();
    rt->SetAnchors( Input::GetMousePositionNDC() );
    rt->SetPivotPosition( Vector2(-1, 1) );
    rt->TranslateLocal( Vector3(0, 0, -0.001f) );

    UIContentSizeFitter *csf = p_rootItem->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    p_rootItem->AddComponent<UILayoutIgnorer>();
    p_rootItem->SetParent(this);

    m_justCreated = true;
}

void Menu::Update()
{
    GameObject::Update();

    if (Input::GetMouseButton(MouseButton::Right) ||
        Input::GetMouseButton(MouseButton::Left))
    {
        if (!m_justCreated &&
            !GetRootItem()->GetRectTransform()->IsMouseOver(true))
        {
            GameObject::Destroy(this);
        }
    }
    m_justCreated = false;
}

MenuItem *Menu::GetRootItem() const
{
    return p_rootItem;
}
