#include "BangEditor/MenuItem.h"

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
    m_itemType = itemType;

    GameObjectFactory::CreateUIGameObjectInto(this);

    SetFontSize(11);

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetChildrenVerticalStretch(Stretch::None);
    vl->SetChildrenHorizontalStretch(Stretch::None);
    vl->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    vl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    if (m_itemType != MenuItemType::Root) { vl->SetPaddings(3); }
    else
    {
        // TODO: Why if I remove this it does not work?
        vl->SetPaddings(1, 1, 0, 0);
    }

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(-1) );
    le->SetPreferredSize( Vector2i(-1) );
    le->SetFlexibleSize( Vector2(0) );

    const Color BgColor = Color::LightGray;
    UIImageRenderer *bg = AddComponent<UIImageRenderer>();
    bg->SetTint(BgColor);

    if (m_itemType != MenuItemType::Root)
    {
        GameObject *textGo = GameObjectFactory::CreateUIGameObject();
        m_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize(12);
        GetText()->SetVerticalAlign(VerticalAlignment::Center);
        GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
        textGo->SetParent(this);
    }

    m_childrenContainer = GameObjectFactory::CreateUIGameObject();
    m_childrenContainer->SetName("m_childrenContainer");
    auto childrenContBg = m_childrenContainer->AddComponent<UIImageRenderer>();
    childrenContBg->SetTint(BgColor);

    auto csf = m_childrenContainer->AddComponent<UIContentSizeFitter>();
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);

    RectTransform *contRT = m_childrenContainer->GetRectTransform();
    m_childrenContainerVL = m_childrenContainer->AddComponent<UIVerticalLayout>();
    m_childrenContainerVL->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    m_childrenContainerVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    contRT->SetAnchors( (itemType == MenuItemType::Top) ? Vector2(-1, -1) :
                                                          Vector2(1, 1));
    contRT->SetPivotPosition(Vector2(-1, 1));

    m_childrenContainer->AddComponent<UILayoutIgnorer>(true);
    m_childrenContainer->SetEnabled(false);
    m_childrenContainer->SetParent(this);

    m_button = AddComponent<UIFocusable>();

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

void MenuItem::SetFontSize(uint fontSize)
{
    if (fontSize != m_fontSize)
    {
        m_fontSize = fontSize;
        List<UITextRenderer*> texts = GetComponentsInChildren<UITextRenderer>();
        for (UITextRenderer *text : texts)
        {
            text->SetTextSize(m_fontSize);
        }
    }
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
    MenuItemType childItemType = m_itemType == MenuItemType::Root ?
                                    MenuItemType::Top : MenuItemType::Normal;
    MenuItem *newItem = GameObject::Create<MenuItem>(childItemType);
    newItem->GetText()->SetContent(text);
    newItem->GetText()->SetTextSize(m_fontSize);
    newItem->SetName(text);
    AddItem(newItem);
    return newItem;
}

UITextRenderer *MenuItem::GetText() const
{
    return m_text;
}

UIFocusable *MenuItem::GetButton() const
{
    return m_button;
}

bool MenuItem::MustDisplayChildren() const
{
    if (m_itemType == MenuItemType::Root) { return true; }

    if (GetButton()->IsMouseOver()) { return true; }
    for (MenuItem *childItem : m_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }
    return false;
}
