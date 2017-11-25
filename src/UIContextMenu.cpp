#include "BangEditor/UIContextMenu.h"

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

    /*
    List<GameObject*> parts = m_parts;
    parts.PushBack(GetGameObject());

    for (GameObject *part : m_parts)
    {
        RectTransform *rt = part->GetRectTransform();
        if (Input::GetMouseButton(MouseButton::Right) && rt->IsMouseOver())
        {
            Menu *menu = GameObject::Create<Menu>();
            PROPAGATE(IUIContextMenuable, OnSetContextMenu, menu);
            menu->SetParent( EditorSceneManager::GetEditorScene() );
            break;
        }
    }
    */
}

void UIContextMenu::AddButtonPart(GameObject *part)
{
    m_parts.PushBack(part);
}

// Menu
Menu::Menu()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UILayoutIgnorer>();

    UIContentSizeFitter *csf = AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors( Input::GetMouseCoordsNDC() );
    rt->SetPivotPosition( Vector2(-1, 1) );

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

MenuItem *Menu::AddItem(const String &text)
{
    MenuItem *item = GameObject::Create<MenuItem>(false);
    item->GetText()->SetContent(text);
    SetAsChild(item);
    return item;
}
