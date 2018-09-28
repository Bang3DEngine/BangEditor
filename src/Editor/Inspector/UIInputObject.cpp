#include "BangEditor/UIInputObject.h"

#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIInputObject::UIInputObject()
{
    SetName("UIInputObject");
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetFlexibleWidth( 1.0f );

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetChildrenVerticalStretch(Stretch::FULL);

    p_objectInputText = GameObjectFactory::CreateUIInputText();
    p_objectInputText->SetBlocked(true);
    p_objectInputText->GetText()->SetTextSize(12);
    p_objectInputText->GetBackground()->SetTint(Color::White);
    UILayoutElement *objectInputTextLE = p_objectInputText->GetGameObject()->
                                         AddComponent<UILayoutElement>();
    objectInputTextLE->SetFlexibleSize( Vector2(9999.9f) );
    objectInputTextLE->SetLayoutPriority(1);

    p_objectInputText->GetGameObject()->SetParent(this);
}

UIInputObject::~UIInputObject()
{
}

void UIInputObject::SetObject(Object *object)
{
    p_object = object;
}

Object *UIInputObject::GetObject() const
{
    return p_object;
}

