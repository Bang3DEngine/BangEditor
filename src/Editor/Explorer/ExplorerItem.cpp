#include "BangEditor/ExplorerItem.h"

#include "Bang/File.h"
#include "Bang/Input.h"
#include "Bang/Dialog.h"
#include "Bang/UILabel.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ImportFilesManager.h"
#include "Bang/UIAspectRatioFitter.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorTextureFactory.h"

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
    iconRT->SetAnchors( Vector2::Zero );
    iconRT->SetPivotPosition( Vector2::Zero );
    p_icon = iconGo->AddComponent<UIImageRenderer>();
    p_icon->SetTint(Color::Zero);

    p_aspectRatioFitter = iconGo->AddComponent<UIAspectRatioFitter>();
    p_aspectRatioFitter->SetAspectRatioMode(AspectRatioMode::KEEP);

    p_label = GameObjectFactory::CreateUILabel();
    GameObject *labelGo = p_label->GetGameObject();
    RectTransform *labelRT = labelGo->GetRectTransform();
    labelRT->SetAnchorX( Vector2(-1,  1) );
    labelRT->SetAnchorY( Vector2(-1, -1) );
    labelRT->SetMarginTop(-textPixels);
    p_label->SetFocusEnabled(false);
    p_label->GetText()->SetTextSize(11);
    p_label->GetText()->SetContent("");
    p_label->GetText()->SetWrapping(true);
    p_label->GetText()->SetVerticalAlign(VerticalAlignment::TOP);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::CENTER);
    p_label->SetSelectable(false);

    p_focusable = AddComponent<UIFocusable>();
    GetFocusable()->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });
    p_contextMenu->AddButtonPart(this);
    p_contextMenu->AddButtonPart(bgGo);

    p_dragDroppable = AddComponent<UIDragDroppable>();


    bgGo->SetParent(this);
    labelGo->SetParent(this);
    iconContainerGo->SetParent(this);
    iconGo->SetParent(iconContainerGo);

    SetSelected(false);
}

ExplorerItem::~ExplorerItem()
{
}

void ExplorerItem::OnEvent(IFocusable*,
                           const UIEvent &event)
{
    if (event.type == UIEvent::Type::MOUSE_ENTER)
    {
        if (!IsSelected())
        {
            p_bg->SetTint(Color::LightBlue.WithAlpha(0.6f));
        }
    }
    else if (event.type == UIEvent::Type::MOUSE_EXIT)
    {
        if (!IsSelected())
        {
            p_bg->SetTint(Color::Zero);
        }
    }
}

void ExplorerItem::SetPath(const Path &path)
{
    if (GetPath() != path)
    {
        m_path = path;

        RH<Texture2D> iconTex = EditorTextureFactory::GetIconForPath(GetPath());
        bool invertY = EditorTextureFactory::IsIconAnImage(GetPath());
        p_icon->SetImageTexture(iconTex.Get());
        p_icon->SetMode(invertY ? UIImageRenderer::Mode::TEXTURE_INV_UVY :
                                  UIImageRenderer::Mode::TEXTURE);
        p_icon->SetTint(Color::White);

        if (iconTex.Get())
        {
            p_aspectRatioFitter->SetAspectRatio( iconTex.Get()->GetSize() );
        }
        else { p_aspectRatioFitter->SetAspectRatio(1.0f); }

        SetPathString(GetPath().GetNameExt());
    }
}

void ExplorerItem::SetSelected(bool selected)
{
    m_selected = selected;
    p_bg->SetTint(IsSelected() ? Color::LightBlue.WithAlpha(0.8f) : Color::Zero);
}

void ExplorerItem::SetPathString(const String &string)
{
    m_pathString = string;
    p_label->GetText()->SetContent( GetPathString() );
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
    return p_focusable;
}

const String &ExplorerItem::GetPathString() const
{
    return m_pathString;
}

void ExplorerItem::Rename()
{
    EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                &IEventsExplorerItem::OnRename, this);
}

void ExplorerItem::Remove()
{
    EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                &IEventsExplorerItem::OnRemove, this);
}

void ExplorerItem::Paste()
{
    EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                &IEventsExplorerItem::OnPastedOver, this);
}

void ExplorerItem::Duplicate()
{
    EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                &IEventsExplorerItem::OnDuplicate, this);
}

void ExplorerItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    if ( GetPathString() != ".." )
    {
        menuRootItem->SetFontSize(12);

        MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
        duplicate->SetSelectedCallback([this](MenuItem*) { Duplicate(); });

        MenuItem *rename = menuRootItem->AddItem("Rename");
        rename->SetSelectedCallback([this](MenuItem*) { Rename(); });

        MenuItem *remove = menuRootItem->AddItem("Remove");
        remove->SetSelectedCallback([this](MenuItem*) { Remove(); });

        MenuItem *copy = menuRootItem->AddItem("Copy");
        copy->SetSelectedCallback([this](MenuItem*)
        {
            EditorClipboard::CopyPath( GetPath() );
        });

        if (GetPath().IsDir())
        {
            MenuItem *pasteItem = menuRootItem->AddItem("Paste");
            pasteItem->SetOverAndActionEnabled( EditorClipboard::HasCopiedPath() );
            pasteItem->SetSelectedCallback([this](MenuItem*) { Paste(); });
        }

    }
}

void ExplorerItem::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    UIDragDroppable *dd = DCAST<UIDragDroppable*>(dd_);
    if (ExplorerItem *expItem = DCAST<ExplorerItem*>(dd->GetGameObject()))
    {
        if (expItem != this &&
            GetRectTransform()->IsMouseOver() &&
            GetPath().IsDir())
        {
            Path newDir = GetPath();
            Path droppedPath = expItem->GetPath();
            File::Rename(droppedPath, newDir.Append(droppedPath.GetNameExt()));

            // Move import file if any
            if ( ImportFilesManager::HasImportFile(droppedPath) )
            {
                Path importDroppedPath =
                        ImportFilesManager::GetImportFilepath(droppedPath);
                File::Rename(importDroppedPath,
                             newDir.Append(importDroppedPath.GetNameExt()));
            }

            EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                           &IEventsExplorerItem::OnDroppedToDirectory, expItem);
        }
    }
}

const Path &ExplorerItem::GetPath() const
{
    return m_path;
}


