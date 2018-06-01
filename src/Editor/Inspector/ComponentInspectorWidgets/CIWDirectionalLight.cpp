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
    p_shadowDistanceInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    AddWidget("Shadow dist.", p_shadowDistanceInput->GetGameObject());

    SetLabelsWidth(110);
}

void CIWDirectionalLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();

    if (!p_shadowDistanceInput->HasFocus())
    {
        p_shadowDistanceInput->SetValue( GetDirectionalLight()->GetShadowDistance() );
    }
}

DirectionalLight *CIWDirectionalLight::GetDirectionalLight() const
{
    return SCAST<DirectionalLight*>( GetLight() );
}

void CIWDirectionalLight::OnValueChanged(EventEmitter<IEventsValueChanged> *object)
{
    CIWLight::OnValueChanged(object);

    GetDirectionalLight()->SetShadowDistance( p_shadowDistanceInput->GetValue() );
}
