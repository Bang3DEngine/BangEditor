#include "BangEditor/CIWSphereCollider.h"

#include "Bang/UIInputNumber.h"
#include "Bang/SphereCollider.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWSphereCollider::CIWSphereCollider()
{
}

CIWSphereCollider::~CIWSphereCollider()
{
}

void CIWSphereCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWSphereCollider");
    SetTitle("SphereCollider");

    p_radiusInput = GameObjectFactory::CreateUIInputNumber();
    p_radiusInput->SetMinMaxValues(0.01f, Math::Infinity<float>());
    p_radiusInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Radius", p_radiusInput->GetGameObject());

    SetLabelsWidth(85);
}

void CIWSphereCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    SphereCollider *sphereCollider = GetSphereCollider();
    if (!p_radiusInput->HasFocus())
    {
        p_radiusInput->SetValue( sphereCollider->GetRadius() );
    }
}

void CIWSphereCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    SphereCollider *sphereCollider = GetSphereCollider();
    sphereCollider->SetRadius( p_radiusInput->GetValue() );
}

SphereCollider *CIWSphereCollider::GetSphereCollider() const
{
    return SCAST<SphereCollider*>( GetCollider() );
}
