#include "BangEditor/CIWDirectionalLight.h"

#include "Bang/DirectionalLight.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWDirectionalLight::Init()
{
    CIWLight::Init();
}

void CIWDirectionalLight::UpdateValuesFromComponent()
{
    CIWLight::UpdateValuesFromComponent();
}

DirectionalLight *CIWDirectionalLight::GetDirectionalLight() const
{
    return SCAST<DirectionalLight*>( GetLight() );
}

void CIWDirectionalLight::OnValueChanged(Object *object)
{
    CIWLight::OnValueChanged(object);
}
