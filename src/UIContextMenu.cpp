#include "BangEditor/UIContextMenu.h"

#include "Bang/Input.h"
#include "Bang/UICanvas.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/UIContentSizeFitter.h"

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
                Menu *menu = GameObject::Create<Menu>();
                EventEmitter<IUIContextMenuable>::
                    PropagateToListeners(&IUIContextMenuable::OnSetContextMenu,
                                         menu->GetRootItem());
                menu->SetParent( EditorSceneManager::GetEditorScene() );
                break;
            }
        }
    }
}

void UIContextMenu::AddButtonPart(GameObject *part)
{
    m_parts.PushBack(part);
}

// Menu
Menu::Menu()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_rootItem = GameObject::Create<MenuItem>( MenuItem::MenuItemType::Root );
    p_rootItem->SetDestroyOnClose(true);
    p_rootItem->SetParent(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors( Input::GetMousePositionNDC() );
    rt->SetPivotPosition( Vector2(-1, 1) );

    UIContentSizeFitter *csf = AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    AddComponent<UILayoutIgnorer>();
    AddComponent<UIVerticalLayout>();
}

void Menu::Update()
{
    GameObject::Update();

    if (Input::GetMouseButton(MouseButton::Right) ||
        Input::GetMouseButton(MouseButton::Left))
    {
        RectTransform *rt = GetRectTransform();
        if (!rt->IsMouseOver(true)) { GameObject::Destroy(this); }
    }
}

MenuItem *Menu::GetRootItem() const
{
    return p_rootItem;
}
