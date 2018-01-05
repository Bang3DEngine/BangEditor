#include "BangEditor/ExplorerItem.h"

#include "Bang/Input.h"
#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
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
    p_icon->SetTint(Color::Zero);

    p_label = GameObjectFactory::CreateUILabel();
    GameObject *labelGo = p_label->GetGameObject();
    RectTransform *labelRT = labelGo->GetRectTransform();
    labelRT->SetAnchorX( Vector2(-1,  1) );
    labelRT->SetAnchorY( Vector2(-1, -1) );
    labelRT->SetMarginTop(-textPixels);
    p_label->GetText()->SetTextSize(9);
    p_label->GetText()->SetContent("");
    p_label->GetText()->SetWrapping(true);
    p_label->GetText()->SetVerticalAlign(VerticalAlignment::Top);
    p_label->SetSelectable(false);

    p_button = AddComponent<UIFocusable>();
    p_button->EventEmitter<IFocusListener>::RegisterListener(this);

    bgGo->SetParent(this);
    iconGo->SetParent(this);
    labelGo->SetParent(this);

    SetSelected(false);
}

ExplorerItem::~ExplorerItem()
{
}

void ExplorerItem::Update()
{
    GameObject::Update();

    if (p_button->IsMouseOver())
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            SetSelected(true);
        }
    }

    if (IsSelected())
    {
        if (Input::GetMouseButtonUp(MouseButton::Left))
        {
            if (!p_button->IsMouseOver()) { SetSelected(false); }
        }
    }
}

void ExplorerItem::SetFilepath(const Path &path)
{
    if (GetPath() != path)
    {
        m_path = path;
        RH<Texture2D> iconTex = EditorIconManager::GetIcon(GetPath());
        p_icon->SetImageTexture(iconTex.Get());
        p_icon->SetTint(Color::White);
        p_label->GetText()->SetContent(GetPath().GetNameExt());
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

UILabel *ExplorerItem::GetLabel() const
{
    return p_label;
}

UIFocusable *ExplorerItem::GetFocusable() const
{
    return p_button;
}

const Path &ExplorerItem::GetPath() const
{
    return m_path;
}

void ExplorerItem::OnMouseEnter(IFocusable*)
{
    if (!IsSelected())
    {
        p_bg->SetTint(Color::VeryLightBlue);
    }
}

void ExplorerItem::OnMouseExit(IFocusable*)
{
    if (!IsSelected())
    {
        p_bg->SetTint(Color::Zero);
    }
}


