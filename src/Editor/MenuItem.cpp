#include "BangEditor/MenuItem.h"

#include "Bang/UIList.h"
#include "Bang/UICanvas.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
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
    SetFontSize(13);
    m_itemType = itemType;

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::NONE);
    hl->SetChildrenVerticalAlignment(VerticalAlignment::CENTER);
    if (GetItemType() != MenuItemType::ROOT) { hl->SetPaddings(3); }
    else
    {
        hl->SetPaddings(1, 1, 0, 0); // TODO: Why if I remove this it does not work?
    }

    if (GetItemType() == MenuItemType::TOP)
    {
        p_topBg = AddComponent<UIImageRenderer>();
    }

    if (GetItemType() != MenuItemType::ROOT)
    {
        hl->SetSpacing(5);

        GameObject *textGo = GameObjectFactory::CreateUIGameObject();
        p_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize( GetFontSize() );
        GetText()->SetVerticalAlign(VerticalAlignment::CENTER);
        GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);

        UILayoutElement *textLE = textGo->AddComponent<UILayoutElement>();
        textLE->SetFlexibleWidth(9999.9f);

        textGo->SetParent(this);

        p_focusable = textGo->AddComponent<UIFocusable>();

        if (GetItemType() != MenuItemType::TOP)
        {
            p_rightArrow = GameObjectFactory::CreateUIImage();
            p_rightArrow->SetImageTexture( TextureFactory::GetRightArrowIcon().Get() );

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

    UIContentSizeFitter *csf = childrenListGo->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::PREFERRED);
    csf->SetVerticalSizeType(LayoutSizeType::PREFERRED);

    AdjustToBeInsideScreen();

    childrenListGo->AddComponent<UILayoutIgnorer>();
    GetChildrenList()->SetSelectionCallback(&MenuItem::OnListSelectionCallback);
    GetChildrenList()->SetSelectedColor( GetChildrenList()->GetOverColor() );
    GetChildrenList()->SetOverColor( Color::LightGray.WithValue(0.9f) );
    GetChildrenList()->SetIdleColor( GetChildrenList()->GetOverColor() );
}

MenuItem::~MenuItem()
{

}

MenuItem* MenuItem::GetTopOrRootItem() const
{
    if (GetItemType() == MenuItemType::ROOT ||
        GetItemType() == MenuItemType::TOP) { return const_cast<MenuItem*>(this); }
    return GetParentItem() ? GetParentItem()->GetTopOrRootItem() : nullptr;
}

bool MenuItem::IsSelected() const
{
    switch (GetItemType())
    {
        case MenuItemType::ROOT: return true;
        case MenuItemType::TOP:  return IsForcedShow();
        case MenuItemType::NORMAL:
        {
            MenuItem *parentItem = GetParentItem();
            if (!parentItem) { return false; }
            return parentItem->GetChildrenList()->GetSelectedItem() == this;
        }
    }
    return true;
}

void MenuItem::AdjustToBeInsideScreen()
{
    MenuItem *topOrRootRT = GetTopOrRootItem();
    if (!topOrRootRT) { return; }

    RectTransform *topRT = topOrRootRT->GetRectTransform();
    AARect topRectNDC = topRT->GetViewportAARectNDC();
    RectTransform *thisListRT = GetChildrenList()->GetGameObject()->GetRectTransform();

    const bool isTop = (GetItemType() == MenuItemType::TOP);
    if (isTop)
    {
        const Vector2 OverlapEpsilon(0.005f);
        thisListRT->SetAnchors(Vector2(-1,-1) + OverlapEpsilon);
        thisListRT->SetPivotPosition(Vector2(-1, 1));
    }
    else
    {
        Vector2 anchors = Vector2::Zero;
        Vector2 pivotPos = thisListRT->GetPivotPosition();
        for (int axis = 0; axis < 2; ++axis) // X and Y
        {
            // Avoid side where it overflows. If it overflows both, just pick 1
            if (topRectNDC.GetCenter()[axis] > 0.0f)
            {
                pivotPos[axis] = 1.0f;
                anchors[axis] = -1;
            }
            else
            {
                pivotPos[axis] = -1.0f;
                anchors[axis] = 1;
            }
        }
        thisListRT->SetAnchors(anchors * Vector2(1,-1));
        thisListRT->SetPivotPosition(pivotPos);
    }

    // TODO: Fix UIContentSizeFitter
    GetChildrenList()->GetGameObject()->GetComponent<UIContentSizeFitter>()->ApplyLayout(Axis::HORIZONTAL);
    GetChildrenList()->GetGameObject()->GetComponent<UIContentSizeFitter>()->ApplyLayout(Axis::VERTICAL);
}

void MenuItem::Update()
{
    GameObject::Update();

    AdjustToBeInsideScreen();

    const bool mustDisplayChildren = MustDisplayChildren();
    GetChildrenList()->GetGameObject()->SetEnabled(mustDisplayChildren);

    if (p_topBg && GetItemType() == MenuItemType::TOP)
    {
        UICanvas *canvas = UICanvas::GetActive(this);
        bool mouseOver = canvas->IsMouseOver(this, true);
        bool tintBg    = mouseOver && !IsForcedShow();

        Color topBgColor = Color::Zero;
        if (tintBg && mouseOver) { topBgColor = Color::VeryLightBlue.WithValue(1.2f); }
        else if (mustDisplayChildren) { topBgColor = Color::VeryLightBlue; }
        else { topBgColor = Color::Zero; }

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
        case UIList::Action::CLICKED_LEFT:
        case UIList::Action::PRESSED:
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

        case UIList::Action::MOUSE_OVER:
            if (parentItem)
            {
                parentItem->GetChildrenList()->SetSelection(menuItem);
            }
        break;

        case UIList::Action::MOUSE_OUT:
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
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5);
    GetChildrenList()->AddItem(sep);
}

void MenuItem::AddItem(MenuItem *childItem)
{
    GetChildrenList()->AddItem(childItem);
    p_childrenItems.PushBack(childItem);
}

MenuItem *MenuItem::AddItem(const String &text)
{
    MenuItem *newItem = GameObject::Create<MenuItem>(MenuItemType::NORMAL);
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
        if (GetItemType() == MenuItemType::TOP)
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
        if (GetItemType() == MenuItemType::NORMAL)
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

    if ( GetItemType() == MenuItemType::ROOT ) { return true; }
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
