#include "BangEditor/MenuItem.h"

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
        p_text = textGo->AddComponent<UITextRenderer>();
        GetText()->SetContent("MenuItem");
        GetText()->SetTextColor(Color::Black);
        GetText()->SetTextSize(12);
        GetText()->SetVerticalAlign(VerticalAlignment::Center);
        GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
        textGo->SetParent(this);
    }

    p_childrenContainer = GameObjectFactory::CreateUIGameObject();
    p_childrenContainer->SetName("m_childrenContainer");
    auto childrenContBg = p_childrenContainer->AddComponent<UIImageRenderer>();
    childrenContBg->SetTint(BgColor);

    auto csf = p_childrenContainer->AddComponent<UIContentSizeFitter>();
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);

    RectTransform *contRT = p_childrenContainer->GetRectTransform();
    p_childrenContainerVL = p_childrenContainer->AddComponent<UIVerticalLayout>();
    p_childrenContainerVL->SetChildrenVerticalAlignment(VerticalAlignment::Bot);
    p_childrenContainerVL->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);
    contRT->SetAnchors( (itemType == MenuItemType::Top) ? Vector2(-1, -1) :
                                                          Vector2(1, 1));
    contRT->SetPivotPosition(Vector2(-1, 1));

    p_childrenContainer->AddComponent<UILayoutIgnorer>(true);
    p_childrenContainer->SetEnabled(false);
    p_childrenContainer->SetParent(this);

    p_button = AddComponent<UIFocusable>();

    GetButton()->AddClickedCallback([this](IFocusable*)
    {
        if (m_itemType == MenuItemType::Normal)
        {
            CloseRecursiveUp();
        }
    });

    SetName("MenuItem");
}

MenuItem::~MenuItem()
{

}

void MenuItem::Update()
{
    GameObject::Update();

    bool mustDisplayChildren = MustDisplayChildren() &&
                               m_canDisplayChildrenThisFrame;
    p_childrenContainer->SetEnabled(mustDisplayChildren);

    GetComponent<UIImageRenderer>()->SetTint(mustDisplayChildren ? Color::White :
                                                                   Color::LightGray);
    m_canDisplayChildrenThisFrame = true; // Reset
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

void MenuItem::SetDestroyOnClose(bool destroyOnClose)
{
    m_destroyOnClose = destroyOnClose;
}

void MenuItem::AddSeparator()
{
    GameObject *sep =
            GameObjectFactory::CreateUIHSeparator(LayoutSizeType::Preferred, 5);
    sep->AddComponent<UIFocusable>();
    sep->SetParent(p_childrenContainer);
}

void MenuItem::AddItem(MenuItem *childItem)
{
    childItem->SetParent(p_childrenContainer);
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

UIFocusable *MenuItem::GetButton() const
{
    return p_button;
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
            p_childrenContainer->SetEnabled(false);
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

    if (GetButton()->IsMouseOver()) { return true; }

    for (MenuItem *childItem : p_childrenItems)
    {
        if (childItem->MustDisplayChildren()) { return true; }
    }

    for (GameObject *child : p_childrenContainer->GetChildren())
    {
        if (UICanvas::GetActive(this)->IsMouseOver(child)) { return true; }
    }

    return false;
}
