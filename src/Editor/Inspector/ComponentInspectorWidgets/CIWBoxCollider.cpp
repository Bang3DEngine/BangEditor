#include "BangEditor/CIWBoxCollider.h"

#include "Bang/BoxCollider.h"
#include "Bang/UIInputNumber.h"

#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWBoxCollider::CIWBoxCollider()
{
}

CIWBoxCollider::~CIWBoxCollider()
{
}

void CIWBoxCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWBoxCollider");
    SetTitle("BoxCollider");

    p_extentsInput = GameObject::Create<UIInputVector>();
    for (UIInputNumber *inputNumber : p_extentsInput->GetInputNumbers())
    {
        inputNumber->SetMinValue(0.01f);
    }
    p_extentsInput->SetSize(3);
    p_extentsInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Extents", p_extentsInput);

    SetLabelsWidth(95);
}

void CIWBoxCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    BoxCollider *boxCollider = GetBoxCollider();
    if (!p_extentsInput->HasFocus())
    {
        p_extentsInput->Set( boxCollider->GetExtents() );
    }
}

void CIWBoxCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    BoxCollider *boxCollider = GetBoxCollider();
    boxCollider->SetExtents( p_extentsInput->GetVector3() );
}

BoxCollider *CIWBoxCollider::GetBoxCollider() const
{
    return SCAST<BoxCollider*>( GetCollider() );
}

