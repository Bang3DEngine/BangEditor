#include "BangEditor/CIWDirectionalLight.h"

#include "Bang/DirectionalLight.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWDirectionalLight::InitInnerWidgets()
{
    CIWLight::InitInnerWidgets();
}

void CIWDirectionalLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();
}

DirectionalLight *CIWDirectionalLight::GetDirectionalLight() const
{
    return SCAST<DirectionalLight*>( GetLight() );
}

void CIWDirectionalLight::OnValueChanged(Object *object)
{
    CIWLight::OnValueChanged(object);
}
