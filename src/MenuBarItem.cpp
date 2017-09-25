#include "BangEditor/MenuBarItem.h"

#include "Bang/UIFrameLayout.h"
#include "Bang/RectTransform.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITintedInteractive.h"
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

    m_buttonWithTint = AddComponent<UITintedInteractive>();
    m_buttonWithTint->AddToTint(this);
    m_buttonWithTint->SetIdleTintColor(BgColor);
    m_buttonWithTint->SetOverTintColor(Color::White);
    m_buttonWithTint->SetPressedTintColor(Color::White);
    m_buttonWithTint->AddListener(this);
    m_buttonWithTint->AddAgent(this);

    m_childrenContainer = GameObjectFactory::CreateUIGameObject();

    UIContentSizeFitter *csf = m_childrenContainer->AddComponent<UIContentSizeFitter>();
    csf->SetVerticalSizeFitMode(SizeFitMode::Sum);
    csf->SetHorizontalSizeFitMode(SizeFitMode::Max);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);

    m_childrenContainerVL = m_childrenContainer->AddComponent<UIVerticalLayout>();
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
    // m_childrenContainer->GetRectTransform()->SetMarginRight(-150);
    // m_childrenContainer->GetRectTransform()->SetMarginBot(-250);
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);
}

MenuBarItem::~MenuBarItem()
{

}

#include "Bang/Scene.h"
#include "Bang/Dialog.h"
#include "Bang/Window.h"
#include "Bang/SceneManager.h"
#include "Bang/UILayoutManager.h"
void MenuBarItem::Update()
{
    UIGameObject::Update();

    if (Input::GetKeyDown(Input::Key::A))
    {
        Window *win = Window::GetCurrent();
        win->SetSize(win->GetWidth(), win->GetHeight());
        UILayoutManager::InvalidateAll(this);
    }
    if (Input::GetKeyDown(Input::Key::E))
    {
        Dialog::Error("ErrorOMG", "Lorem ipsum dolor sit amet consecteur"
                                  " adiscipilim. Bangerino Pizzerino con un"
                                  " poco de oregano.");
    }
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

void MenuBarItem::OnButton_MouseEnter(UIInteractive *btn)
{
    m_childrenContainer->SetEnabled(true);
}

void MenuBarItem::OnButton_MouseExit(UIInteractive *btn)
{
    m_childrenContainer->SetEnabled(false);
}

void MenuBarItem::OnButton_Clicked(UIInteractive *btn)
{

}
