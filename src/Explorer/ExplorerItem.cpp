#include "BangEditor/ExplorerItem.h"

#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIButtoneable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ExplorerItem::ExplorerItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetPreferredSize( Vector2i(100) );

    constexpr int textPixels = 30;
    constexpr int spacing = 5;

    GameObject *bgGo = GameObjectFactory::CreateUIGameObject();
    p_bg = bgGo->AddComponent<UIImageRenderer>();

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconRT = iconGo->GetRectTransform();
    iconRT->SetAnchorX( Vector2(-1,  1) );
    iconRT->SetAnchorY( Vector2(-1,  1) );
    iconRT->SetMarginBot(textPixels + spacing);
    p_icon = iconGo->AddComponent<UIImageRenderer>();
    p_icon->SetAspectRatioMode(AspectRatioMode::Keep);

    p_label = GameObjectFactory::CreateUILabel();
    GameObject *labelGo = p_label->GetGameObject();
    RectTransform *labelRT = labelGo->GetRectTransform();
    labelRT->SetAnchorX( Vector2(-1,  1) );
    labelRT->SetAnchorY( Vector2(-1, -1) );
    labelRT->SetMarginTop(-textPixels);
    p_label->GetText()->SetTextSize(9);
    p_label->GetText()->SetContent("");
    p_label->GetText()->SetWrapping(true);
    p_label->GetText()->SetVerticalAlign(VerticalAlignment::Center);
    p_label->SetSelectable(false);

    p_button = AddComponent<UIButtoneable>();
    p_button->RegisterButtonPart(this);
    p_button->SetMode(UIButtoneableMode::RectTransform);
    p_button->EventEmitter<IUIButtonListener>::RegisterListener(this);

    SetAsChild(bgGo);
    SetAsChild(iconGo);
    SetAsChild(labelGo);

    SetSelected(false);
}

ExplorerItem::~ExplorerItem()
{
}

void ExplorerItem::Update()
{
    GameObject::Update();

    if (IsSelected())
    {
        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            if (!UICanvas::IsMouseOver(this)) { SetSelected(false); }
        }
    }
}

void ExplorerItem::SetFilepath(const Path &path)
{
    if (GetFilepath() != path)
    {
        m_filepath = path;
        p_icon->SetImageTexture( EditorIconManager::GetIcon(GetFilepath()) );
        p_label->GetText()->SetContent(GetFilepath().GetNameExt());
    }
}

void ExplorerItem::SetSelected(bool selected)
{
    m_selected = selected;
    p_bg->SetTint(IsSelected() ? Color::LightBlue : Color::Zero);
}

bool ExplorerItem::IsSelected() const
{
    return m_selected;
}

const Path &ExplorerItem::GetFilepath() const
{
    return m_filepath;
}

void ExplorerItem::OnButton_MouseEnter(UIButtoneable *btn)
{
    if (!IsSelected())
    {
        p_bg->SetTint(Color::VeryLightBlue);
    }
}

void ExplorerItem::OnButton_MouseExit(UIButtoneable *btn)
{
    if (!IsSelected())
    {
        p_bg->SetTint(Color::Zero);
    }
}

void ExplorerItem::OnButton_Clicked(UIButtoneable *btn)
{
    SetSelected(true);
}

void ExplorerItem::OnButton_DoubleClicked(UIButtoneable*)
{
    if (GetFilepath().IsDir())
    {
        Explorer::GetInstance()->SetCurrentPath(GetFilepath());
    }
}

