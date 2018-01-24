#include "BangEditor/HierarchyItem.h"

#include "Bang/Scene.h"
#include "Bang/UITree.h"
#include "Bang/UILabel.h"
#include "Bang/UIFocusable.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorClipboard.h"
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/EditorSceneManager.h"

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

    AddComponent<UIHorizontalLayout>();

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    textGo->SetName("HierarchyItemText");
    p_textRenderer = textGo->AddComponent<UITextRenderer>();
    p_textRenderer->SetTextSize(10);
    p_textRenderer->SetHorizontalAlign(HorizontalAlignment::Left);
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
                    EventEmitter<INameListener>::UnRegisterListener(
                                                SCAST<INameListener*>(this) );
        }

        p_refGameObject = referencedGameObject;

        SetText( GetReferencedGameObject()->GetName() );
        GetReferencedGameObject()->
                        EventEmitter<INameListener>::RegisterListener(this);
    }
}

GameObject *HierarchyItem::GetReferencedGameObject() const
{
    return p_refGameObject;
}

void HierarchyItem::CreateEmpty()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnCreateEmpty, this);
}

void HierarchyItem::Rename()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnRename, this);
}

void HierarchyItem::Remove()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnRemove, this);
}

void HierarchyItem::Copy()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnCopy, this);
}

void HierarchyItem::Cut()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnCut, this);
}

void HierarchyItem::Paste()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnPaste, this);
}

void HierarchyItem::Duplicate()
{
    EventEmitter<IHierarchyItemListener>::PropagateToListeners(
                &IHierarchyItemListener::OnDuplicate, this);
}

void HierarchyItem::OnNameChanged(GameObject *go, const String &,
                                  const String &newName)
{
    ASSERT(go == GetReferencedGameObject());
    SetText( newName );
}

void HierarchyItem::OnCreateContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(10);

    MenuItem *createEmpty = menuRootItem->AddItem("Create Empty");
    createEmpty->SetSelectedCallback([this](MenuItem*)
    { CreateEmpty(); });

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
        case UIList::Action::SelectionIn:
            selectGameObject = true;
        break;

        case UIList::Action::ClickedRight:
            selectGameObject = true;
            p_contextMenu->ShowMenu();
        break;

        case UIList::Action::DoubleClickedLeft:
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
