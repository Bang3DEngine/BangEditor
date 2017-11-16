#include "BangEditor/MenuBarItem.h"

#include "Bang/RectTransform.h"
#include "Bang/UITintedButton.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG_EDITOR

MenuBarItem::MenuBarItem(bool topItem)
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::None);
    vl->SetChildrenHorizontalStretch(Stretch::None);
    vl->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);

    if (topItem) { vl->SetPaddings(2); }
    else         { vl->SetPaddings(3); }

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

    m_childrenContainer =
            GameObjectFactory::CreateUIGameObjectNamed("m_childrenContainer");
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

    if (topItem)
    {
        contRT->SetAnchors(Vector2(-1, -1));
        contRT->SetPivotPosition(Vector2(-1, 1));
    }
    else
    {
        contRT->SetAnchors(Vector2(1, 1));
        contRT->SetPivotPosition(Vector2(-1, 1));
    }
    m_childrenContainer->AddComponent<UILayoutIgnorer>(true);
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);

    m_button = AddComponent<UIButton>();
    m_button->SetMode(UIButtonMode::UseRectTransform);
    m_button->RegisterButtonPart(this);

    SetName("MenuBarItem");
}

MenuBarItem::~MenuBarItem()
{

}

void MenuBarItem::Update()
{
    GameObject::Update();

    bool mustDisplayChildren = MustDisplayChildren();
    m_childrenContainer->SetEnabled(mustDisplayChildren);

    GetComponent<UIImageRenderer>()->SetTint(mustDisplayChildren ? Color::White :
                                                                   Color::Zero);
}

void MenuBarItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    sep->SetParent(m_childrenContainer);
}

void MenuBarItem::AddChild(MenuBarItem *childItem)
{
    m_childrenContainer->SetAsChild(childItem);
    m_childrenItems.PushBack(childItem);
}

MenuBarItem *MenuBarItem::AddChild(const String &text)
{
    MenuBarItem *newItem = GameObject::Create<MenuBarItem>(false);
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
    return m_button;
}

bool MenuBarItem::MustDisplayChildren() const
{
    if (GetButton()->IsMouseOverSomePart()) { return true; }
    for (MenuBarItem *childItem : m_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }
    return false;
}
