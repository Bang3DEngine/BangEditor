#include "BangEditor/CIWDirectionalLight.h"

#include "Bang/UISlider.h"
#include "Bang/UIInputNumber.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWDirectionalLight::InitInnerWidgets()
{
    CIWLight::InitInnerWidgets();

    SetName("CIWDirectionalLight");
    SetTitle("DirectionalLight");

    p_shadowDistanceInput = GameObjectFactory::CreateUIInputNumber();
    p_shadowDistanceInput->SetMinMaxValues(1.0f, 2000.0f);
    p_shadowDistanceInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Shadow dist.", p_shadowDistanceInput->GetGameObject());

    p_shadowMapSizeInput = GameObjectFactory::CreateUISlider();
    p_shadowMapSizeInput->SetMinMaxValues(1.0f, 4096);
    p_shadowMapSizeInput->GetInputNumber()->SetDecimalPlaces(0);
    p_shadowMapSizeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Shadow map size", p_shadowMapSizeInput->GetGameObject());

    SetLabelsWidth(110);
}

void CIWDirectionalLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();

    if (!p_shadowDistanceInput->HasFocus())
    {
        p_shadowDistanceInput->SetValue( GetDirectionalLight()->GetShadowDistance() );
    }

    if (!p_shadowMapSizeInput->HasFocus())
    {
        p_shadowMapSizeInput->SetValue( GetDirectionalLight()->GetShadowMapSize().x );
    }
}

DirectionalLight *CIWDirectionalLight::GetDirectionalLight() const
{
    return SCAST<DirectionalLight*>( GetLight() );
}

void CIWDirectionalLight::OnValueChanged(Object *object)
{
    CIWLight::OnValueChanged(object);

    GetDirectionalLight()->SetShadowDistance( p_shadowDistanceInput->GetValue() );
    GetDirectionalLight()->SetShadowMapSize( Vector2i(p_shadowMapSizeInput->GetValue()) );
}
