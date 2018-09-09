#include "BangEditor/HierarchyItem.h"

#include "Bang/Scene.h"
#include "Bang/UITree.h"
#include "Bang/UILabel.h"
#include "Bang/UIFocusable.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Editor.h"
#include "BangEditor/MenuBar.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

HierarchyItem::HierarchyItem()
{
    SetName("HierarchyItem");

    GameObjectFactory::CreateUIGameObjectInto(this);
    AddComponent<UIFocusable>();

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        OnCreateContextMenu(menuRootItem);
    });

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

void HierarchyItem::OnStart()
{
    GameObject::OnStart();
    GetComponent<UIContextMenu>()->AddButtonPart(GetParent());
}

void HierarchyItem::Update()
{
    GameObject::Update();
}

void HierarchyItem::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (referencedGameObject != GetReferencedGameObject())
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->
                    EventEmitter<IEventsName>::UnRegisterListener(
                                SCAST<EventListener<IEventsName>*>(this) );
        }

        p_refGameObject = referencedGameObject;

        SetText( GetReferencedGameObject()->GetName() );
        GetReferencedGameObject()->
                        EventEmitter<IEventsName>::RegisterListener(this);
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

void HierarchyItem::OnNameChanged(GameObject *go, const String &,
                                  const String &newName)
{
    ASSERT(go == GetReferencedGameObject());
    SetText( newName );
}

void HierarchyItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(12);

    MenuItem *create = menuRootItem->AddItem("Create");
    MenuBar::CreateGameObjectMenuInto(create);

    MenuItem *createPrefab = menuRootItem->AddItem("Create Prefab");
    createPrefab->SetSelectedCallback([this](MenuItem*)
    { CreatePrefab(); });

    menuRootItem->AddSeparator();

    MenuItem *copy = menuRootItem->AddItem("Copy");
    copy->SetSelectedCallback([this](MenuItem*) { Copy(); });

    MenuItem *cut = menuRootItem->AddItem("Cut");
    cut->SetSelectedCallback([this](MenuItem*) { Cut(); });

    MenuItem *paste = menuRootItem->AddItem("Paste");
    paste->SetSelectedCallback([this](MenuItem*) { Paste(); });
    paste->SetOverAndActionEnabled( EditorClipboard::HasCopiedGameObject() );

    MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
    duplicate->SetSelectedCallback([this](MenuItem*) { Duplicate(); });

    menuRootItem->AddSeparator();

    MenuItem *rename = menuRootItem->AddItem("Rename");
    rename->SetSelectedCallback([this](MenuItem*) { Rename(); });

    menuRootItem->AddSeparator();

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->SetSelectedCallback([this](MenuItem*) { Remove(); });
}

void HierarchyItem::SetText(const String &text)
{
    if (text != m_text)
    {
        m_text = text;
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
        break;

        case UIList::Action::MOUSE_RIGHT_DOWN:
            selectGameObject = true;
            p_contextMenu->ShowMenu();
        break;

        case UIList::Action::DOUBLE_CLICKED_LEFT:
        {
            Hierarchy *h = Hierarchy::GetInstance();
            UITree *t = h->GetUITree();
            t->SetItemCollapsed(this, !t->IsItemCollapsed(this));
        }
        break;

        default: break;
    }

    if (selectGameObject)
    {
        Editor::SelectGameObject(refGo);
    }
}

String HierarchyItem::ToString() const
{
    GameObject *refGo = GetReferencedGameObject();
    return "HItem(" + ( refGo ? refGo->GetName() : "Null" ) + ")";
}
