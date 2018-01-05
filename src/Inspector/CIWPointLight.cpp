#include "BangEditor/CIWPointLight.h"

#include "Bang/PointLight.h"
#include "Bang/UIInputNumber.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWPointLight::Init()
{
    CIWLight::Init();

    p_rangeInput = GameObjectFactory::CreateUIInputNumber();
    p_rangeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Range", p_rangeInput->GetGameObject());
}

void CIWPointLight::UpdateValuesFromComponent()
{
    CIWLight::UpdateValuesFromComponent();

    if (!p_rangeInput->HasFocus())
    {
        p_rangeInput->SetValue( GetPointLight()->GetRange() );
    }
}

PointLight *CIWPointLight::GetPointLight() const
{
    return SCAST<PointLight*>( GetLight() );
}

void CIWPointLight::OnValueChanged(Object *object)
{
    CIWLight::OnValueChanged(object);

    GetPointLight()->SetRange( p_rangeInput->GetValue() );
}
