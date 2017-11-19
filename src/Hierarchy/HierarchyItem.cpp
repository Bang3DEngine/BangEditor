#include "BangEditor/HierarchyItem.h"

#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

HierarchyItem::HierarchyItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIContextMenu *ctxMenu = AddComponent<UIContextMenu>();
    ctxMenu->EventEmitter<IUIContextMenuable>::RegisterListener(this);

    p_bg = AddComponent<UIImageRenderer>();
    p_bg->SetTint(Color::Zero);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);

    p_label = GameObjectFactory::CreateUILabel();
    p_label->GetText()->SetTextSize(10);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
    p_label->SetSelectable(false);
    SetText("HierarchyItem");

    SetAsChild(p_label->GetGameObject());
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
        p_refGameObject = referencedGameObject;
        p_label->GetText()->SetContent( GetReferencedGameObject()->GetName() );
    }
}

GameObject *HierarchyItem::GetReferencedGameObject() const
{
    return p_refGameObject;
}

void HierarchyItem::OnSetContextMenu(Menu *menu)
{
    MenuItem *hola = menu->AddItem("Hola");
    hola->AddItem("Holita");
    hola->AddItem("Hehe");
    menu->AddItem("Adios");
}

void HierarchyItem::SetText(const String &text)
{
    m_text = text;
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
            Hierarchy::GetInstance()->ToggleItemCollapsed(this);
        break;

        default: break;
    }
}

String HierarchyItem::ToString() const
{
    GameObject *refGo = GetReferencedGameObject();
    return "HItem(" + ( refGo ? refGo->GetName() : "Null" ) + ")";
}
