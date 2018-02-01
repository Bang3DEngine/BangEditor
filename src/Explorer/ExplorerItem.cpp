#include "BangEditor/ExplorerItem.h"

#include "Bang/File.h"
#include "Bang/Input.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/UIAspectRatioFitter.h"

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

    GameObject *iconContainerGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconContainerRT = iconContainerGo->GetRectTransform();
    iconContainerRT->SetMarginBot(textPixels + spacing);

    GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
    RectTransform *iconRT = iconGo->GetRectTransform();
    iconRT->SetAnchorX( Vector2::Zero );
    iconRT->SetAnchorY( Vector2::Zero );
    iconRT->SetPivotPosition( Vector2::Zero );
    p_icon = iconGo->AddComponent<UIImageRenderer>();
    p_icon->SetTint(Color::Zero);

    p_aspectRatioFitter = iconGo->AddComponent<UIAspectRatioFitter>();
    p_aspectRatioFitter->SetAspectRatioMode(AspectRatioMode::Keep);

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

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    p_contextMenu->AddButtonPart(this);
    p_contextMenu->AddButtonPart(bgGo);

    bgGo->SetParent(this);
    labelGo->SetParent(this);
    iconContainerGo->SetParent(this);
    iconGo->SetParent(iconContainerGo);

    SetSelected(false);
}

ExplorerItem::~ExplorerItem()
{
}

void ExplorerItem::Update()
{
    GameObject::Update();
}

void ExplorerItem::SetPath(const Path &path)
{
    if (GetPath() != path)
    {
        m_path = path;

        RH<Texture2D> iconTex = EditorIconManager::GetIconForPath(GetPath());
        p_icon->SetImageTexture(iconTex.Get());
        p_icon->SetTint(Color::White);

        if (iconTex.Get())
        {
            p_aspectRatioFitter->SetAspectRatio( iconTex.Get()->GetSize() );
        }
        else { p_aspectRatioFitter->SetAspectRatio(1.0f); }

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

void ExplorerItem::Rename()
{
    EventEmitter<IExplorerItemListener>::PropagateToListeners(
                &IExplorerItemListener::OnRename, this);
}

void ExplorerItem::Remove()
{
    EventEmitter<IExplorerItemListener>::PropagateToListeners(
                &IExplorerItemListener::OnRemove, this);
}

void ExplorerItem::Duplicate()
{
    EventEmitter<IExplorerItemListener>::PropagateToListeners(
                &IExplorerItemListener::OnDuplicate, this);
}

void ExplorerItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(10);

    MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
    duplicate->SetSelectedCallback([this](MenuItem*) { Duplicate(); });

    MenuItem *rename = menuRootItem->AddItem("Rename");
    rename->SetSelectedCallback([this](MenuItem*) { Rename(); });

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*) { Remove(); });
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


