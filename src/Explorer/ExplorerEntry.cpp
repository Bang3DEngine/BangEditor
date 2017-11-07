#include "BangEditor/ExplorerEntry.h"

#include "Bang/UILabel.h"
#include "Bang/RectTransform.h"
#include "Bang/UITintedButton.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/IconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ExplorerEntry::ExplorerEntry()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinSize( Vector2i(50) );

    constexpr int textPixels = 10;

    GameObject *bgGo = GameObjectFactory::CreateUIGameObject();
    p_bg = bgGo->AddComponent<UIImageRenderer>();

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconRT = iconGo->GetComponent<RectTransform>();
    iconRT->SetAnchorX( Vector2(-1,  1) );
    iconRT->SetAnchorY( Vector2(-1,  1) );
    iconRT->SetMarginBot(textPixels);
    p_icon = iconGo->AddComponent<UIImageRenderer>();
    p_icon->SetAspectRatioMode(AspectRatioMode::Keep);

    p_label = GameObjectFactory::CreateUILabel();
    GameObject *labelGo = p_label->GetGameObject();
    RectTransform *labelRT = labelGo->GetComponent<RectTransform>();
    labelRT->SetAnchorX( Vector2(-1,  1) );
    labelRT->SetAnchorY( Vector2(-1, -1) );
    labelRT->SetMarginTop(-textPixels);
    p_label->GetText()->SetTextSize(9);
    p_label->GetText()->SetContent("");
    p_label->GetText()->SetWrapping(true);
    p_label->GetText()->SetVerticalAlign(VerticalAlignment::Top);
    p_label->SetSelectable(false);

    p_button = AddComponent<UITintedButton>();
    p_button->SetMode(UIButtonMode::UseRectTransform);
    p_button->SetIdleTintColor(Color::Zero);
    p_button->SetOverTintColor(Color::VeryLightBlue);
    p_button->SetPressedTintColor(Color::LightBlue);
    p_button->AddToTint(p_bg);
    p_button->RegisterEmitter(this);
    p_button->EventEmitter<IUIButtonListener>::RegisterListener(this);

    AddChild(bgGo);
    AddChild(iconGo);
    AddChild(labelGo);
}

ExplorerEntry::~ExplorerEntry()
{
}

void ExplorerEntry::Update()
{
    GameObject::Update();

    if (IsSelected())
    {
        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            RectTransform *rt = GetComponent<RectTransform>();
            if (!rt->IsMouseOver()) { SetSelected(false); }
        }
    }
}

void ExplorerEntry::SetFilepath(const Path &path)
{
    if (GetFilepath() != path)
    {
        m_filepath = path;
        p_icon->SetImageTexture( IconManager::GetIcon(GetFilepath()) );
        p_label->GetText()->SetContent(GetFilepath().GetName());
    }
}

void ExplorerEntry::SetSelected(bool selected)
{
    m_selected = selected;
    p_button->SetTintEnabled( !IsSelected() );
    p_bg->SetTint(IsSelected() ? Color::LightBlue : Color::Zero);
}

bool ExplorerEntry::IsSelected() const
{
    return m_selected;
}

const Path &ExplorerEntry::GetFilepath() const
{
    return m_filepath;
}

void ExplorerEntry::OnButton_Clicked(UIButton *btn)
{
    SetSelected(true);
}

void ExplorerEntry::OnButton_DoubleClicked(UIButton*)
{
    if (GetFilepath().IsDir())
    {
        Explorer::GetInstance()->SetCurrentFilepath(GetFilepath());
    }
}

