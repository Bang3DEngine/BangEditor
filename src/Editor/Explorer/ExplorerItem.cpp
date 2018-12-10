#include "BangEditor/ExplorerItem.h"

#include "Bang/Alignment.h"
#include "Bang/AspectRatioMode.h"
#include "Bang/Color.h"
#include "Bang/DPtr.tcc"
#include "Bang/File.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/Key.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/RectTransform.h"
#include "Bang/Texture2D.h"
#include "Bang/UIAspectRatioFitter.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITheme.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/IEventsExplorerItem.h"
#include "BangEditor/MenuItem.h"

using namespace Bang;
using namespace BangEditor;

ExplorerItem::ExplorerItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback(
        [this](MenuItem *menuRootItem) { OnCreateContextMenu(menuRootItem); });
    p_contextMenu->SetFocusable(GetFocusable());

    p_dragDroppable = AddComponent<UIDragDroppable>();
    p_dragDroppable->SetFocusable(GetFocusable());
}

ExplorerItem::~ExplorerItem()
{
}

void ExplorerItem::OnFocusTaken(UIFocusable *, const UIEvent &event)
{
    if (event.focus.type == FocusType::AUTO_TAB)
    {
        if (Explorer *exp = Explorer::GetInstance())
        {
            exp->SelectPath(GetPath());
        }
    }
}

UIEventResult ExplorerItem::OnUIEvent(UIFocusable *focusable,
                                      const UIEvent &event)
{
    if (NavigatorItem::OnUIEvent(focusable, event) == UIEventResult::INTERCEPT)
    {
        return UIEventResult::INTERCEPT;
    }

    switch (event.type)
    {
        case UIEvent::Type::FOCUS_TAKEN:
            SetSelected(true);
            return UIEventResult::INTERCEPT;
            break;

        case UIEvent::Type::MOUSE_CLICK_FULL:
            if (Explorer *exp = Explorer::GetInstance())
            {
                exp->SelectPath(GetPath());
                return UIEventResult::INTERCEPT;
            }
            break;

        case UIEvent::Type::KEY_DOWN:
            if (event.key.modifiers == KeyModifier::LCTRL)
            {
                switch (event.key.key)
                {
                    case Key::D:
                        Duplicate();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::C:
                        EditorClipboard::CopyPath(GetPath());
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::V:
                        Paste();
                        return UIEventResult::INTERCEPT;
                        break;

                    default: break;
                }
            }
            else
            {
                switch (event.key.key)
                {
                    case Key::F2:
                        Rename();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::DELETE:
                        Remove();
                        return UIEventResult::INTERCEPT;
                        break;

                    default: break;
                }
            }
            break;

        default: break;
    }
    return UIEventResult::IGNORE;
}

void ExplorerItem::SetPath(const Path &path)
{
    if (GetPath() != path)
    {
        m_path = path;

        Texture2D *iconTex = EditorTextureFactory::GetIconForPath(GetPath());
        bool invertY = EditorTextureFactory::IsIconAnImage(GetPath());
        GetIcon()->SetImageTexture(iconTex);
        GetIcon()->SetMode(invertY ? UIImageRenderer::Mode::TEXTURE_INV_UVY
                                   : UIImageRenderer::Mode::TEXTURE);
        GetIcon()->SetTint(EditorTextureFactory::GetPathIconTint(GetPath()));

        if (iconTex)
        {
            GetAspectRatioFitter()->SetAspectRatio(iconTex->GetSize());
        }
        else
        {
            GetAspectRatioFitter()->SetAspectRatio(1.0f);
        }

        SetPathString(GetPath().GetNameExt());
    }
}

void ExplorerItem::SetPathString(const String &string)
{
    m_pathString = string;
    GetLabel()->GetText()->SetContent(GetPathString());
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
    if (GetPathString() != "..")
    {
        menuRootItem->SetFontSize(12);

        MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
        duplicate->SetSelectedCallback([this](MenuItem *) { Duplicate(); });

        MenuItem *rename = menuRootItem->AddItem("Rename");
        rename->SetSelectedCallback([this](MenuItem *) { Rename(); });

        MenuItem *remove = menuRootItem->AddItem("Remove");
        remove->SetSelectedCallback([this](MenuItem *) { Remove(); });

        MenuItem *copy = menuRootItem->AddItem("Copy");
        copy->SetSelectedCallback(
            [this](MenuItem *) { EditorClipboard::CopyPath(GetPath()); });

        if (GetPath().IsDir())
        {
            MenuItem *pasteItem = menuRootItem->AddItem("Paste");
            pasteItem->SetOverAndActionEnabled(
                EditorClipboard::HasCopiedPath());
            pasteItem->SetSelectedCallback([this](MenuItem *) { Paste(); });
        }
    }
}

void ExplorerItem::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDrop(dd_);

    if (!GetRectTransform()->IsMouseOver(true))
    {
        return;
    }

    UIDragDroppable *dd = DCAST<UIDragDroppable *>(dd_);
    if (ExplorerItem *expItem = DCAST<ExplorerItem *>(dd->GetGameObject()))
    {
        if (expItem != this && GetRectTransform()->IsMouseOver() &&
            GetPath().IsDir())
        {
            Path newDir = GetPath();
            Path droppedPath = expItem->GetPath();
            if (droppedPath.Exists())
            {
                File::Rename(droppedPath,
                             newDir.Append(droppedPath.GetNameExt()));

                // Move import file if any
                if (MetaFilesManager::HasMetaFile(droppedPath))
                {
                    Path metaDroppedPath =
                        MetaFilesManager::GetMetaFilepath(droppedPath);
                    File::Rename(metaDroppedPath,
                                 newDir.Append(metaDroppedPath.GetNameExt()));
                }

                EventEmitter<IEventsExplorerItem>::PropagateToListeners(
                    &IEventsExplorerItem::OnDroppedToDirectory, expItem);
            }
        }
    }
}

const Path &ExplorerItem::GetPath() const
{
    return m_path;
}
