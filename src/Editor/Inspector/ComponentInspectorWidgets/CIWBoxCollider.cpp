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

    p_halfExtentsInput = GameObject::Create<UIInputVector>();
    for (UIInputNumber *inputNumber : p_halfExtentsInput->GetInputNumbers())
    {
        inputNumber->SetMinMaxValues(0.01f, Math::Infinity<float>());
    }
    p_halfExtentsInput->SetSize(3);
    p_halfExtentsInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Half Extents", p_halfExtentsInput);

    SetLabelsWidth(85);
}

void CIWBoxCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    BoxCollider *boxCollider = GetBoxCollider();
    if (!p_halfExtentsInput->HasFocus())
    {
        p_halfExtentsInput->Set( boxCollider->GetHalfExtents() );
    }
}

void CIWBoxCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    BoxCollider *boxCollider = GetBoxCollider();
    boxCollider->SetHalfExtents( p_halfExtentsInput->GetVector3() );
}

BoxCollider *CIWBoxCollider::GetBoxCollider() const
{
    return SCAST<BoxCollider*>( GetCollider() );
}

