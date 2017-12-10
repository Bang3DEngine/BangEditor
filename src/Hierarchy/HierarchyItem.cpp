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
#include "BangEditor/EditorIconManager.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

HierarchyItem::HierarchyItem()
{
    SetName("HierarchyItem");

    GameObjectFactory::CreateUIGameObjectInto(this);
    AddComponent<UIFocusable>();

    UIContextMenu *ctxMenu = AddComponent<UIContextMenu>();
    ctxMenu->EventEmitter<IUIContextMenuable>::RegisterListener(this);

    p_bg = AddComponent<UIImageRenderer>();
    p_bg->SetTint(Color::Zero);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);

    GameObject *textGo = GameObjectFactory::CreateUIGameObject();
    textGo->SetName("HierarchyItemText");
    p_textRenderer = textGo->AddComponent<UITextRenderer>();
    p_textRenderer->SetTextSize(10);
    p_textRenderer->SetHorizontalAlign(HorizontalAlignment::Left);
    SetText("HierarchyItem");

    SetAsChild(textGo);
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

void HierarchyItem::OnNameChanged(GameObject *go, const String &,
                                  const String &newName)
{
    ASSERT(go == GetReferencedGameObject());
    SetText( newName );
}

void HierarchyItem::OnSetContextMenu(MenuItem *menuRootItem)
{
    menuRootItem->SetFontSize(10);

    MenuItem *createEmpty = menuRootItem->AddItem("Create Empty");
    createEmpty->GetButton()->AddClickedCallback([this](IFocusable*)
    {
        GameObject *empty = GameObjectFactory::CreateGameObjectNamed("Empty");
        GetReferencedGameObject()->SetAsChild(empty);
    });

    MenuItem *duplicate = menuRootItem->AddItem("Duplicate");
    duplicate->GetButton()->AddClickedCallback([this](IFocusable*)
    {
        GameObject *original = GetReferencedGameObject();
        GameObject *parent = original->GetParent();
        int originalIndex = parent->GetChildren().IndexOf(original);
        ASSERT(originalIndex != -1);

        GameObject *clone = original->Clone();
        clone->SetParent(parent, originalIndex+1);
        clone->SetName( GameObjectFactory::GetGameObjectDuplicateName(original) );

        // Collapse as clone
        Hierarchy *h = Hierarchy::GetInstance();
        HierarchyItem *originalItem = h->GetItemFromGameObject(original);
        HierarchyItem *cloneItem = h->GetItemFromGameObject(clone);
        bool isOriginalCollapsed = h->GetUITree()->IsItemCollapsed(originalItem);
        h->GetUITree()->SetItemCollapsed(cloneItem, isOriginalCollapsed);
    });

    menuRootItem->AddSeparator();

    MenuItem *remove = menuRootItem->AddItem("Remove");
    remove->GetButton()->AddClickedCallback([this](IFocusable*)
    {
        GameObject::Destroy( GetReferencedGameObject() );
    });

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
    switch (action)
    {
        case UIList::Action::MouseOver:
            if (!m_isSelected) { p_bg->SetTint(Color::VeryLightBlue); }
        break;

        case UIList::Action::MouseOut:
            if (!m_isSelected) { p_bg->SetTint(Color::Zero); }
        break;

        case UIList::Action::SelectionIn:
            m_isSelected = true;
            p_bg->SetTint(Color::LightBlue);
            Editor::SelectGameObject(refGo);
        break;

        case UIList::Action::SelectionOut:
            m_isSelected = false;
            p_bg->SetTint(Color::Zero);
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
}

String HierarchyItem::ToString() const
{
    GameObject *refGo = GetReferencedGameObject();
    return "HItem(" + ( refGo ? refGo->GetName() : "Null" ) + ")";
}
