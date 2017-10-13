#include "BangEditor/MenuBarItem.h"

#include "Bang/UIBorderRect.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG_EDITOR

MenuBarItem::MenuBarItem(bool topItem)
{
    AddComponent<RectTransform>();

    UIFrameLayout *fl = AddComponent<UIFrameLayout>();
    fl->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    fl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);

    m_isTopItem = topItem;
    if (m_isTopItem) { fl->SetPaddings(2); }
    else             { fl->SetPaddings(3); }

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(-1) );
    le->SetPreferredSize( Vector2i(-1) );
    le->SetFlexibleSize( Vector2(0) );

    const Color BgColor = Color::LightGray;
    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(BgColor);

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    m_text = textGo->AddComponent<UITextRenderer>();
    GetText()->SetContent("MenuItem");
    GetText()->SetTextColor(Color::Black);
    GetText()->SetTextSize(12);
    GetText()->SetVerticalAlign(VerticalAlignment::Center);
    GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    textGo->SetParent(this);

    m_buttonWithTint = AddComponent<UITintedButton>();
    m_buttonWithTint->AddToTint(this);
    m_buttonWithTint->SetIdleTintColor(BgColor);
    m_buttonWithTint->SetOverTintColor(Color::White);
    m_buttonWithTint->SetPressedTintColor(Color::White);
    m_buttonWithTint->SetMode(UIButtonMode::UseRender);
    m_buttonWithTint->AddListener(this);
    m_buttonWithTint->AddAgent(this);

    m_childrenContainer = GameObjectFactory::CreateUIGameObject();
    auto childrenContBg = m_childrenContainer->AddComponent<UIImageRenderer>();
    childrenContBg->SetTint(BgColor);

    auto csf = m_childrenContainer->AddComponent<UIContentSizeFitter>();
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);

    RectTransform *contRT = m_childrenContainer->GetComponent<RectTransform>();
    m_childrenContainerVL = m_childrenContainer->AddComponent<UIVerticalLayout>();
    m_childrenContainerVL->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    m_childrenContainerVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    contRT->SetPivotPosition(Vector2(-1));
    if (m_isTopItem)
    {
        contRT->SetAnchors(Vector2(-1, -1));
        contRT->SetPivotPosition(Vector2(-1, 1));
    }
    else
    {
        contRT->SetAnchors(Vector2(1, 1));
        contRT->SetPivotPosition(Vector2(-1, 1));
    }
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);
}

MenuBarItem::~MenuBarItem()
{

}

void MenuBarItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    sep->SetParent(m_childrenContainer);
}

void MenuBarItem::AddChild(MenuBarItem *childItem)
{
    childItem->SetParent(m_childrenContainer);
    m_childrenItems.PushBack(childItem);
}

MenuBarItem *MenuBarItem::AddChild(const String &text)
{
    MenuBarItem *newItem = new MenuBarItem(false);
    newItem->GetText()->SetContent(text);
    newItem->SetName(text);
    AddChild(newItem);
    return newItem;
}

UITextRenderer *MenuBarItem::GetText() const
{
    return m_text;
}

UIButton *MenuBarItem::GetButton() const
{
    return m_buttonWithTint;
}

void MenuBarItem::OnButton_MouseEnter(UIButton *btn)
{
    m_childrenContainer->SetEnabled(true);
}

void MenuBarItem::OnButton_MouseExit(UIButton *btn)
{
    m_childrenContainer->SetEnabled(false);
}

void MenuBarItem::OnButton_Clicked(UIButton *btn)
{
    if (!m_isTopItem) { parent->SetEnabled(false); }
}
