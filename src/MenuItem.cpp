#include "BangEditor/MenuItem.h"

#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG_EDITOR

MenuItem::MenuItem(bool topItem)
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::None);
    vl->SetChildrenHorizontalStretch(Stretch::None);
    vl->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    vl->SetPaddings(3);

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

    RectTransform *contRT = m_childrenContainer->GetRectTransform();
    m_childrenContainerVL = m_childrenContainer->AddComponent<UIVerticalLayout>();
    m_childrenContainerVL->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    m_childrenContainerVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    contRT->SetAnchors(topItem ? Vector2(-1, -1) : Vector2(1, 1));
    contRT->SetPivotPosition(Vector2(-1, 1));

    m_childrenContainer->AddComponent<UILayoutIgnorer>(true);
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);

    m_button = AddComponent<UIButtoneable>();
    m_button->SetMode(UIButtoneableMode::RectTransform);
    m_button->RegisterButtonPart(this);
    m_button->RegisterButtonPart(m_childrenContainer);

    SetName("MenuItem");
}

MenuItem::~MenuItem()
{

}

void MenuItem::Update()
{
    GameObject::Update();

    bool mustDisplayChildren = MustDisplayChildren();
    m_childrenContainer->SetEnabled(mustDisplayChildren);

    GetComponent<UIImageRenderer>()->SetTint(mustDisplayChildren ? Color::White :
                                                                   Color::LightGray);
}

void MenuItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    sep->SetParent(m_childrenContainer);
}

void MenuItem::AddItem(MenuItem *childItem)
{
    m_childrenContainer->SetAsChild(childItem);
    m_childrenItems.PushBack(childItem);
}

MenuItem *MenuItem::AddItem(const String &text)
{
    MenuItem *newItem = GameObject::Create<MenuItem>(false);
    newItem->GetText()->SetContent(text);
    newItem->SetName(text);
    AddItem(newItem);
    return newItem;
}

UITextRenderer *MenuItem::GetText() const
{
    return m_text;
}

UIButtoneable *MenuItem::GetButton() const
{
    return m_button;
}

bool MenuItem::MustDisplayChildren() const
{
    if (GetButton()->IsMouseOverSomePart()) { return true; }
    for (MenuItem *childItem : m_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }
    return false;
}
