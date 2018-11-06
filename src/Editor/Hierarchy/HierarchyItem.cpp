#include "BangEditor/HierarchyItem.h"

#include "Bang/Alignment.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsName.h"
#include "Bang/Key.h"
#include "Bang/Stretch.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/IEventsHierarchyItem.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"

namespace Bang
{
class IEventsObject;
class Object;
}

using namespace Bang;
using namespace BangEditor;

HierarchyItem::HierarchyItem()
{
    SetName("HierarchyItem");

    GameObjectFactory::CreateUIGameObjectInto(this);
    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback(
        [this](MenuItem *menuRootItem) { OnCreateContextMenu(menuRootItem); });
    p_contextMenu->SetFocusable(p_focusable);

    UIHorizontalLayout *hLayout = AddComponent<UIHorizontalLayout>();
    hLayout->SetChildrenHorizontalStretch(Stretch::FULL);

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    textGo->SetName("HierarchyItemText");
    p_textRenderer = textGo->AddComponent<UITextRenderer>();
    p_textRenderer->SetTextSize(12);
    p_textRenderer->SetHorizontalAlign(HorizontalAlignment::LEFT);
    SetText("HierarchyItem");

    textGo->SetParent(this);
}

HierarchyItem::~HierarchyItem()
{
}

void HierarchyItem::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (referencedGameObject != GetReferencedGameObject())
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()
                ->EventEmitter<IEventsName>::UnRegisterListener(
                    SCAST<EventListener<IEventsName> *>(this));
        }

        p_refGameObject = referencedGameObject;

        UpdateEnabledDisabledColor();
        SetText(GetReferencedGameObject()->GetName());

        GetReferencedGameObject()
            ->EventEmitter<IEventsObject>::RegisterListener(this);
        GetReferencedGameObject()->EventEmitter<IEventsName>::RegisterListener(
            this);
        SetName("HItem_" + GetReferencedGameObject()->GetName());
    }
}

GameObject *HierarchyItem::GetReferencedGameObject() const
{
    return p_refGameObject;
}

void HierarchyItem::Rename()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnRename, this);
}

void HierarchyItem::Remove()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnRemove, this);
}

void HierarchyItem::Copy()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnCopy, this);
}

void HierarchyItem::Cut()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnCut, this);
}

void HierarchyItem::Paste()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnPaste, this);
}

void HierarchyItem::Duplicate()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnDuplicate, this);
}

void HierarchyItem::CreatePrefab()
{
    EventEmitter<IEventsHierarchyItem>::PropagateToListeners(
        &IEventsHierarchyItem::OnCreatePrefab, this);
}

void HierarchyItem::UpdateEnabledDisabledColor()
{
    if (GetReferencedGameObject()->IsEnabledRecursively())
    {
        p_textRenderer->SetTextColor(Color::Black());
    }
    else
    {
        p_textRenderer->SetTextColor(Color::DarkGray());
    }
}

UIFocusable *HierarchyItem::GetFocusable() const
{
    return p_focusable;
}

void HierarchyItem::OnEnabled(Object *obj)
{
    GameObject::OnEnabled(obj);
    UpdateEnabledDisabledColor();
}

void HierarchyItem::OnDisabled(Object *obj)
{
    GameObject::OnDisabled(obj);
    UpdateEnabledDisabledColor();
}

void HierarchyItem::OnNameChanged(GameObject *go,
                                  const String &,
                                  const String &newName)
{
    ASSERT(go == GetReferencedGameObject());
    SetText(newName);
}

void HierarchyItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *create = menuRootItem->AddItem("Create");
    MenuBar::CreateGameObjectCreateMenuInto(create);

    MenuItem *createPrefab = menuRootItem->AddItem("Create Prefab");
    createPrefab->SetSelectedCallback([this](MenuItem *) { CreatePrefab(); });

    menuRootItem->AddSeparator();

    MenuItem *copy = menuRootItem->AddItem("Copy");
    copy->SetSelectedCallback([this](MenuItem *) { Copy(); });

    MenuItem *cut = menuRootItem->AddItem("Cut");
    cut->SetSelectedCallback([this](MenuItem *) { Cut(); });

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem *) { Paste(); });
    paste->SetOverAndActionEnabled(EditorClipboard::HasCopiedGameObject());

    MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
    duplicate->SetSelectedCallback([this](MenuItem *) { Duplicate(); });

    menuRootItem->AddSeparator();

    MenuItem *rename = menuRootItem->AddItem("Rename");
    rename->SetSelectedCallback([this](MenuItem *) { Rename(); });

    menuRootItem->AddSeparator();

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem *) { Remove(); });

    menuRootItem->AddSeparator();
    MenuBar::CreateGameObjectMiscMenuInto(menuRootItem);
}

UIFocusable *HierarchyItem::GetTreeItemFocusable()
{
    return GetFocusable();
}

void HierarchyItem::SetText(const String &text)
{
    if (text != p_textRenderer->GetContent())
    {
        p_textRenderer->SetContent(text);
    }
}

void HierarchyItem::OnSelectionCallback(UIList::Action action)
{
    GameObject *refGo = GetReferencedGameObject();

    bool selectGameObject = false;
    switch (action)
    {
        case UIList::Action::SELECTION_IN:
            selectGameObject = true;
            UICanvas::GetActive(this)->SetFocus(GetTreeItemFocusable());
            break;

        case UIList::Action::MOUSE_RIGHT_DOWN:
            selectGameObject = true;
            p_contextMenu->ShowMenu();
            break;

        case UIList::Action::DOUBLE_CLICKED_LEFT:
        {
            if (EditorCamera *edCam = EditorCamera::GetInstance())
            {
                if (GameObject *refGo = GetReferencedGameObject())
                {
                    edCam->LookAt(refGo);
                }
            }
        }
        break;

        default: break;
    }

    if (selectGameObject)
    {
        Editor::SelectGameObject(refGo);
    }
}

UIEventResult HierarchyItem::OnUIEvent(UIFocusable *, const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::KEY_DOWN:
            if (event.key.modifiers.IsOn(KeyModifier::LCTRL))
            {
                switch (event.key.key)
                {
                    case Key::C:
                        Copy();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::X:
                        Cut();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::V:
                        Paste();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::D:
                        Duplicate();
                        return UIEventResult::INTERCEPT;
                        break;

                    default: break;
                }
            }
            else
            {
                switch (event.key.key)
                {
                    case Key::F:
                        if (EditorCamera *edCam = EditorCamera::GetInstance())
                        {
                            edCam->LookAt(GetReferencedGameObject());
                        }
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::DELETE:
                        Remove();
                        return UIEventResult::INTERCEPT;
                        break;

                    case Key::F2:
                        Rename();
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

String HierarchyItem::ToString() const
{
    GameObject *refGo = GetReferencedGameObject();
    return "HItem(" + (refGo ? refGo->GetName() : "Null") + ")";
}
