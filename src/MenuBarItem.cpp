#include "BangEditor/MenuBarItem.h"

#include "Bang/UIFrameLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG_EDITOR

MenuBarItem::MenuBarItem(bool toTheRight)
{
    RectTransform *rt = AddComponent<RectTransform>();
    UIFrameLayout *fl = AddComponent<UIFrameLayout>();
    fl->SetChildrenVerticalAlignment(VerticalAlignment::Center);
    fl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    fl->SetPaddingLeft(5);
    fl->SetPaddingRight(5);
    fl->SetPaddingTop(4);
    fl->SetPaddingBot(4);

    const Color BgColor = Color::LightGray;
    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(BgColor);

    UIGameObject *textGo = GameObjectFactory::CreateUIGameObject();
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
    m_buttonWithTint->AddListener(this);
    m_buttonWithTint->AddAgent(this);

    m_childrenContainer = GameObjectFactory::CreateUIGameObject();

    UIImageRenderer *childrenContainerBg = m_childrenContainer->AddComponent<UIImageRenderer>();
    m_childrenContainer->SetName("BG");
    childrenContainerBg->SetTint(BgColor);

    UIContentSizeFitter *csf = m_childrenContainer->AddComponent<UIContentSizeFitter>();
    csf->SetVerticalSizeFitMode(SizeFitMode::Sum);
    csf->SetHorizontalSizeFitMode(SizeFitMode::Max);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);

    m_childrenContainerVL = m_childrenContainer->AddComponent<UIVerticalLayout>();
    m_childrenContainerVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    m_childrenContainerVL->SetChildrenHorizontalStretch(Stretch::Full);
    m_childrenContainer->GetRectTransform()->SetPivotPosition(Vector2(-1));
    if (toTheRight)
    {
        m_childrenContainer->GetRectTransform()->SetAnchors(Vector2(1, 1));
        m_childrenContainer->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
    }
    else
    {
        m_childrenContainer->GetRectTransform()->SetAnchors(Vector2(-1, -1));
        m_childrenContainer->GetRectTransform()->SetPivotPosition(Vector2(-1,1));
    }
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);
}

MenuBarItem::~MenuBarItem()
{

}

void MenuBarItem::AddSeparator()
{
    UIGameObject *sep =
            GameObjectFactory::CreateGUIHSeparator(LayoutSizeType::Preferred, 5);
    sep->SetName("SEP");
    sep->SetParent(m_childrenContainer);
}

void MenuBarItem::AddChild(MenuBarItem *childItem)
{
    childItem->SetParent(m_childrenContainer);
    m_childrenItems.PushBack(childItem);
}

MenuBarItem *MenuBarItem::AddChild(const String &text)
{
    MenuBarItem *newItem = new MenuBarItem(true);
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

}
