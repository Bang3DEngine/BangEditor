#include "BangEditor/MenuItem.h"

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

USING_NAMESPACE_BANG_EDITOR

MenuItem::MenuItem(MenuItemType itemType)
{
    SetName("MenuItem");
    m_itemType = itemType;
    SetFontSize(11);

    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalStretch(Stretch::None);
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    if (m_itemType != MenuItemType::Root) { hl->SetPaddings(3); }
    else
    {
        // TODO: Why if I remove this it does not work?
        hl->SetPaddings(1, 1, 0, 0);
    }

    if (m_itemType == MenuItemType::Top)
    {
        p_topBg = AddComponent<UIImageRenderer>();
    }

    if (m_itemType != MenuItemType::Root)
    {
        GameObject *textGo = GameObjectFactory::CreateUIGameObject();
        p_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize( m_fontSize );
        GetText()->SetVerticalAlign(VerticalAlignment::Center);
        GetText()->SetHorizontalAlign(HorizontalAlignment::Left);

        p_focusable = textGo->AddComponent<UIFocusable>();
        GetFocusable()->AddClickedCallback([this](IFocusable*)
        { if (m_itemType == MenuItemType::Normal) { CloseRecursiveUp(); } });

        textGo->SetParent(this);
    }

    p_childrenList = GameObjectFactory::CreateUIList(false);
    p_childrenList->GetGameObject()->SetName("ChildrenList");
    p_childrenList->GetGameObject()->SetParent(this);

    RectTransform *contRT = p_childrenList->GetGameObject()->GetRectTransform();
    const bool isTop = (itemType == MenuItemType::Top);
    contRT->SetAnchors(isTop ? Vector2(-1, -1) : Vector2(1, 1));
    contRT->SetPivotPosition(Vector2(-1, 1));

    UIContentSizeFitter *csf = p_childrenList->GetGameObject()->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    p_childrenList->GetGameObject()->AddComponent<UILayoutIgnorer>(true);
    p_childrenList->SetSelectionCallback(&MenuItem::OnListSelectionCallback);
    p_childrenList->SetUseSelectedColor(false);
}

MenuItem::~MenuItem()
{

}

void MenuItem::OnListSelectionCallback(GameObject *item, UIList::Action action)
{
    MenuItem *menuItem = DCAST<MenuItem*>(item);
    switch (action)
    {
        case UIList::Action::ClickedLeft:
        case UIList::Action::ClickedRight:
        case UIList::Action::Pressed:
        {
            if (menuItem) { menuItem->GetFocusable()->Click(false); }
        }
        break;

        case UIList::Action::MouseOver:
            if (menuItem) { menuItem->m_itemOverInList = true; }
        break;

        case UIList::Action::MouseOut:
            if (menuItem)  { menuItem->m_itemOverInList = false; }
        break;

        default: break;
    };

}

void MenuItem::Update()
{
    GameObject::Update();

    bool mustDisplayChildren = MustDisplayChildren() &&
                               m_canDisplayChildrenThisFrame;
    p_childrenList->GetGameObject()->SetEnabled(mustDisplayChildren);

    if (m_itemType == MenuItemType::Top)
    {
        p_topBg->SetTint( mustDisplayChildren ? Color::VeryLightBlue :
                                                Color::Zero );
    }

    m_canDisplayChildrenThisFrame = true; // Reset
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

void MenuItem::SetDestroyOnClose(bool destroyOnClose)
{
    m_destroyOnClose = destroyOnClose;
}

void MenuItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    p_childrenList->AddItem(sep);
}

void MenuItem::AddItem(MenuItem *childItem)
{
    p_childrenList->AddItem(childItem);
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

UITextRenderer *MenuItem::GetText() const
{
    return p_text;
}

UIFocusable *MenuItem::GetFocusable() const
{
    return p_focusable;
}

void MenuItem::CloseRecursiveUp()
{
    if (!m_destroyOnClose)
    {
        if (p_parentItem)
        {
            p_parentItem->CloseRecursiveUp();
        }
        else
        {
            p_childrenList->GetGameObject()->SetEnabled(false);
            m_canDisplayChildrenThisFrame = false;
        }
    }
    else
    {
        GameObject::Destroy(this);
    }
}

bool MenuItem::MustDisplayChildren() const
{
    if (m_itemType == MenuItemType::Root) { return true; }
    if (m_itemOverInList) { return true; }

    if (p_childrenList->IsEnabled(true) &&
        p_childrenList->GetGameObject()->GetRectTransform()->IsMouseOver(false))
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

    if (GetFocusable()->IsMouseOver())
    {
        return true;
    }

    for (MenuItem *childItem : p_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }

    for (GameObject *child : p_childrenList->GetItems())
    {
        if (UICanvas::GetActive(this)->IsMouseOver(child)) { return true; }
    }

    return false;
}
