#include "BangEditor/MenuItem.h"

#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/IconManager.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG_EDITOR

MenuItem::MenuItem(MenuItemType itemType)
{
    SetName("MenuItem");
    m_itemType = itemType;
    SetFontSize(11);

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::None);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    if (m_itemType != MenuItemType::Root) { hl->SetPaddings(3); }
    else
    {
        hl->SetPaddings(1, 1, 0, 0); // TODO: Why if I remove this it does not work?
    }

    if (m_itemType == MenuItemType::Top)
    {
        p_topBg = AddComponent<UIImageRenderer>();
    }

    if (m_itemType != MenuItemType::Root)
    {
        hl->SetSpacing(5);

        GameObject *textGo = GameObjectFactory::CreateUIGameObject();
        p_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize( m_fontSize );
        GetText()->SetVerticalAlign(VerticalAlignment::Center);
        GetText()->SetHorizontalAlign(HorizontalAlignment::Left);

        UILayoutElement *textLE = textGo->AddComponent<UILayoutElement>();
        textLE->SetFlexibleWidth(9999.9f);

        textGo->SetParent(this);

        p_focusable = textGo->AddComponent<UIFocusable>();

        if (m_itemType != MenuItemType::Top)
        {
            p_rightArrow = GameObjectFactory::CreateUIImage();
            p_rightArrow->SetImageTexture( IconManager::GetRightArrowIcon().Get() );

            UILayoutElement *rightArrowLE = p_rightArrow->GetGameObject()
                                            ->AddComponent<UILayoutElement>();
            rightArrowLE->SetPreferredSize( Vector2i(7, 14) );

            p_rightArrow->GetGameObject()->SetParent(this);
        }
    }

    p_childrenList = GameObjectFactory::CreateUIList(false);
    GameObject *childrenListGo = GetChildrenList()->GetGameObject();
    childrenListGo->SetName("ChildrenList");
    childrenListGo->SetParent(this);

    RectTransform *contRT = childrenListGo->GetRectTransform();
    constexpr float OverlapEpsilon = 0.005f;
    const bool isTop = (itemType == MenuItemType::Top);
    contRT->SetAnchors(isTop ? Vector2(-1 + OverlapEpsilon, -1 + OverlapEpsilon) :
                               Vector2( 1 - OverlapEpsilon,  1 - OverlapEpsilon));
    contRT->SetPivotPosition(Vector2(-1, 1));

    UIContentSizeFitter *csf = childrenListGo->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    childrenListGo->AddComponent<UILayoutIgnorer>();
    GetChildrenList()->SetSelectionCallback(&MenuItem::OnListSelectionCallback);
    GetChildrenList()->SetSelectedColor( GetChildrenList()->GetOverColor() );
    GetChildrenList()->SetOverColor( Color::LightGray.WithValue(0.9f) );
    GetChildrenList()->SetIdleColor( GetChildrenList()->GetOverColor() );
}

MenuItem::~MenuItem()
{

}

bool MenuItem::IsSelectedInList() const
{
    return p_parentItem ? (p_parentItem->GetChildrenList()->GetSelectedItem() == this) :
                          false;
}

void MenuItem::Update()
{
    GameObject::Update();

    Color textColor = Color::Black;
    UICanvas *canvas = UICanvas::GetActive(this);
    if (!IsMenuEnabled() &&
        m_itemType == MenuItemType::Top &&
        canvas->IsMouseOver(this, true))
    {
        textColor = Color::White;
    }
    GetText()->SetTextColor(textColor);

    const bool mustDisplayChildren = MustDisplayChildren();
    GetChildrenList()->GetGameObject()->SetEnabled(mustDisplayChildren);

    if (p_topBg)
    {
        p_topBg->SetTint( mustDisplayChildren ? Color::VeryLightBlue :
                                                Color::Zero);
    }
    if (p_rightArrow) { p_rightArrow->SetEnabled( !p_childrenItems.IsEmpty() ); }
    if (m_justClosed > 0) { --m_justClosed; }
}

void MenuItem::OnListSelectionCallback(GameObject *item, UIList::Action action)
{
    MenuItem *menuItem = DCAST<MenuItem*>(item);
    MenuItem *parentItem = menuItem ? menuItem->p_parentItem : nullptr;
    switch (action)
    {
        case UIList::Action::ClickedLeft:
        case UIList::Action::Pressed:
        {
            if (menuItem && menuItem->m_selectedCallback)
            {
                menuItem->m_selectedCallback(menuItem);
                menuItem->CloseRecursiveUp();
                menuItem->m_justClosed = 5;
            }
        }
        break;

        case UIList::Action::MouseOver:
            if (parentItem) { parentItem->GetChildrenList()->SetSelection(menuItem); }
        break;

        case UIList::Action::MouseOut:
            if (parentItem) { parentItem->GetChildrenList()->SetSelection(nullptr); }
        break;

        default: break;
    };

}

void MenuItem::SetFontSize(uint fontSize)
{
    if (fontSize != m_fontSize)
    {
        m_fontSize = fontSize;
        List<UITextRenderer*> texts = GetComponentsInChildren<UITextRenderer>();
        for (UITextRenderer *text : texts) { text->SetTextSize(m_fontSize); }
    }
}

void MenuItem::CloseRecursiveUp()
{
    if (!p_parentItem)
    {
        if (!m_destroyOnClose)
        {
            GetChildrenList()->GetGameObject()->SetEnabled(false);
        }
        else
        {
            GameObject::Destroy(this);
        }
    }
    else
    {
        p_parentItem->CloseRecursiveUp();
        p_parentItem->GetChildrenList()->SetSelection(nullptr);
    }
}
void MenuItem::SetDestroyOnClose(bool destroyOnSelect)
{
    m_destroyOnClose = destroyOnSelect;
}

void MenuItem::SetSelectedCallback(MenuItem::ItemSelectedCallback selectedCallback)
{
    m_selectedCallback = selectedCallback;
}

void MenuItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    GetChildrenList()->AddItem(sep);
}

void MenuItem::AddItem(MenuItem *childItem)
{
    GetChildrenList()->AddItem(childItem);
    p_childrenItems.PushBack(childItem);
}

MenuItem *MenuItem::AddItem(const String &text)
{
    MenuItem *newItem = GameObject::Create<MenuItem>(MenuItemType::Normal);
    newItem->GetText()->SetContent(text);
    newItem->GetText()->SetTextSize(m_fontSize);
    newItem->SetName(text);
    newItem->p_parentItem = this;
    AddItem(newItem);
    return newItem;
}

void MenuItem::SetMenuEnabled(bool enabled)
{
    if (enabled != IsMenuEnabled())
    {
        m_menuEnabled = enabled;
    }
}

bool MenuItem::IsMenuEnabled() const
{
    return m_menuEnabled;
}

UIList *MenuItem::GetChildrenList() const
{
    return p_childrenList;
}

UITextRenderer *MenuItem::GetText() const
{
    return p_text;
}

UIFocusable *MenuItem::GetFocusable() const
{
    return p_focusable;
}

bool MenuItem::MustDisplayChildren() const
{
    if (m_justClosed > 0) { return false; }
    if (!IsMenuEnabled()) { return false; }

    if (m_itemType == MenuItemType::Root) { return true; }
    if ( IsSelectedInList() ) { return true; }

    if (GetChildrenList()->IsEnabled(true) &&
        GetChildrenList()->GetGameObject()->GetRectTransform()->IsMouseOver(false))
    {
        return true;
    }

    if (m_itemType == MenuItemType::Top)
    {
        if (GetRectTransform()->IsMouseOver(false))
        {
            return true;
        }
    }

    for (MenuItem *childItem : p_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }

    return false;
}
