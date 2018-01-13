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
    SetFontSize(11);
    m_itemType = itemType;

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::None);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    if (GetItemType() != MenuItemType::Root) { hl->SetPaddings(3); }
    else
    {
        hl->SetPaddings(1, 1, 0, 0); // TODO: Why if I remove this it does not work?
    }

    if (GetItemType() == MenuItemType::Top)
    {
        p_topBg = AddComponent<UIImageRenderer>();
    }

    if (GetItemType() != MenuItemType::Root)
    {
        hl->SetSpacing(5);

        GameObject *textGo = GameObjectFactory::CreateUIGameObject();
        p_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize( GetFontSize() );
        GetText()->SetVerticalAlign(VerticalAlignment::Center);
        GetText()->SetHorizontalAlign(HorizontalAlignment::Left);

        UILayoutElement *textLE = textGo->AddComponent<UILayoutElement>();
        textLE->SetFlexibleWidth(9999.9f);

        textGo->SetParent(this);

        p_focusable = textGo->AddComponent<UIFocusable>();

        if (GetItemType() != MenuItemType::Top)
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

bool MenuItem::IsSelected() const
{
    switch (GetItemType())
    {
        case MenuItemType::Root: return true;
        case MenuItemType::Top: return IsForcedShow();
        case MenuItemType::Normal:
        {
            MenuItem *parentItem = GetParentItem();
            if (!parentItem) { return false; }
            return parentItem->GetChildrenList()->GetSelectedItem() == this;;
        }
    }
    return true;
}

void MenuItem::Update()
{
    GameObject::Update();

    const bool mustDisplayChildren = MustDisplayChildren();
    GetChildrenList()->GetGameObject()->SetEnabled(mustDisplayChildren);

    if (GetItemType() == MenuItemType::Top)
    {
        UICanvas *canvas = UICanvas::GetActive(this);
        bool mouseOver = canvas->IsMouseOver(this, true);
        bool tintWhite = mouseOver && !IsForcedShow();
        GetText()->SetTextColor((tintWhite && mouseOver) ? Color::White :
                                                           Color::Black);
    }

    if (p_topBg)
    {
        Color topBgColor = mustDisplayChildren ? Color::VeryLightBlue : Color::Zero;
        p_topBg->SetTint(topBgColor);
    }
    if (p_rightArrow) { p_rightArrow->SetEnabled( !GetChildrenItems().IsEmpty() ); }
}

void MenuItem::OnListSelectionCallback(GameObject *item, UIList::Action action)
{
    MenuItem *menuItem = DCAST<MenuItem*>(item);
    MenuItem *parentItem = menuItem ? menuItem->GetParentItem() : nullptr;
    switch (action)
    {
        case UIList::Action::ClickedLeft:
        case UIList::Action::Pressed:
        {
            if (menuItem && menuItem->IsOverAndActionEnabled() &&
                menuItem->m_selectedCallback)
            {
                menuItem->m_selectedCallback(menuItem);
                UICanvas::GetActive(menuItem)->SetFocus(nullptr);
                menuItem->Close(true);
            }
        }
        break;

        case UIList::Action::MouseOver:
            if (parentItem)
            {
                parentItem->GetChildrenList()->SetSelection(menuItem);
            }
        break;

        case UIList::Action::MouseOut:
            if (parentItem && menuItem->GetChildrenItems().IsEmpty() )
            {
                parentItem->GetChildrenList()->SetSelection(nullptr);
            }
        break;

        default: break;
    };

}

void MenuItem::SetFontSize(uint fontSize)
{
    if (fontSize != GetFontSize())
    {
        m_fontSize = fontSize;
        List<UITextRenderer*> texts = GetComponentsInChildren<UITextRenderer>();
        for (UITextRenderer *text : texts) { text->SetTextSize(GetFontSize()); }
    }
}

void MenuItem::Close(bool recursiveUp)
{
    if (!GetParentItem() || !recursiveUp)
    {
        if (!GetDestroyOnClose())
        {
            GetChildrenList()->GetGameObject()->SetEnabled(false);
            GetChildrenList()->SetSelection(nullptr);
            for (MenuItem *childItem : GetChildrenItems()) { childItem->Close(false); }
        }
        else
        {
            GameObject::Destroy(this);
        }
    }
    else
    {
        GetParentItem()->Close(true);
        GetParentItem()->GetChildrenList()->SetSelection(nullptr);
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
    newItem->GetText()->SetTextSize(GetFontSize());
    newItem->SetName(text);
    newItem->p_parentItem = this;
    AddItem(newItem);
    return newItem;
}

void MenuItem::SetForceShow(bool forceShow)
{
    if (forceShow != IsForcedShow())
    {
        m_forcedShow = forceShow;
        if (GetItemType() == MenuItemType::Top)
        {
            if (IsForcedShow()) { GetText()->SetTextColor(Color::Black); }
            if (!IsForcedShow()) { p_topBg->SetTint(Color::Zero); }
        }
    }
}

void MenuItem::SetOverAndActionEnabled(bool enabled)
{
    if (enabled != IsOverAndActionEnabled())
    {
        m_overAndActionEnabled = enabled;
        if (GetItemType() == MenuItemType::Normal)
        {
            GetText()->SetTextColor(IsOverAndActionEnabled() ? Color::Black :
                                                               Color::DarkGray);
        }
    }
}

void MenuItem::SetOverAndActionEnabledRecursively(bool enabled)
{
    SetOverAndActionEnabled(enabled);
    for (MenuItem *childItem : GetChildrenItems())
    {
        childItem->SetOverAndActionEnabledRecursively(enabled);
    }
}

void MenuItem::SetDropDownEnabled(bool enabled)
{
    if (enabled != IsDropDownEnabled())
    {
        m_dropDownEnabled = true;
    }
}

void MenuItem::SetDropDownEnabledRecursively(bool enabled)
{
    SetDropDownEnabled(enabled);
    for (MenuItem *childItem : GetChildrenItems())
    {
        childItem->SetDropDownEnabledRecursively(enabled);
    }
}

bool MenuItem::MustDisplayChildren() const
{
    if (!IsDropDownEnabled()) { return false; }

    if ( GetItemType() == MenuItemType::Root ) { return true; }
    if ( IsForcedShow() ) { return true; }
    if ( IsSelected() ) { return true; }

    return false;
}

bool MenuItem::IsOverAndActionEnabled() const { return m_overAndActionEnabled; }
bool MenuItem::IsDropDownEnabled() const { return m_dropDownEnabled; }
UIList *MenuItem::GetChildrenList() const { return p_childrenList; }
UITextRenderer *MenuItem::GetText() const { return p_text; }
UIFocusable *MenuItem::GetFocusable() const { return p_focusable; }
MenuItem::MenuItemType MenuItem::GetItemType() const { return m_itemType; }
bool MenuItem::GetDestroyOnClose() const { return m_destroyOnClose; }
float MenuItem::GetFontSize() const { return m_fontSize; }
bool MenuItem::IsForcedShow() const { return m_forcedShow; }
MenuItem *MenuItem::GetParentItem() const { return p_parentItem; }
const List<MenuItem *> &MenuItem::GetChildrenItems() const { return p_childrenItems; }
