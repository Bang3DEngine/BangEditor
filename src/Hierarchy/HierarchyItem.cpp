#include "BangEditor/HierarchyItem.h"

#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Hierarchy.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

HierarchyItem::HierarchyItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

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

void HierarchyItem::SetText(const String &text)
{
    m_text = text;
}

void HierarchyItem::OnSelectionCallback(UIList::Action action)
{
    GameObject *refGo = GetReferencedGameObject();
    switch (action)
    {
        case UIList::Action::SelectionIn:
            p_bg->SetTint(Color::LightBlue);
            Editor::SelectGameObject(refGo);
        break;

        case UIList::Action::SelectionOut:
            p_bg->SetTint(Color::Zero);
        break;

        default: break;
    }
}

String HierarchyItem::ToString() const
{
    GameObject *refGo = GetReferencedGameObject();
    return "HItem(" + ( refGo ? refGo->GetName() : "Null" ) + ")";
}
