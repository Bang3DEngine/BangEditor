#include "BangEditor/HierarchyItem.h"

#include "Bang/UILabel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

HierarchyItem::HierarchyItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenHorizontalAlignment(HorizontalAlignment::Left);

    p_label = GameObjectFactory::CreateUILabel();
    p_label->GetText()->SetTextSize(10);
    p_label->GetText()->SetHorizontalAlign(HorizontalAlignment::Left);
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

}
