#include "BangEditor/CIWLight.h"

#include "Bang/Light.h"
#include "Bang/UISlider.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

#include "Bang/Debug.h"
void CIWLight::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWLight");
    SetTitle("Light");

    p_intensityInput = GameObjectFactory::CreateUIInputNumber();
    p_intensityInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    p_intensityInput->SetMinMaxValues(0.0f, Math::Infinity<float>());
    AddWidget("Intensity", p_intensityInput->GetGameObject());

    p_colorInput = GameObject::Create<UIInputColor>();
    p_colorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Color", p_colorInput);

    p_shadowBiasInput = GameObjectFactory::CreateUISlider();
    p_shadowBiasInput->SetMinMaxValues(0.0f, 0.1f);
    p_shadowBiasInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Shadow bias", p_shadowBiasInput->GetGameObject());

    p_shadowTypeInput = GameObjectFactory::CreateUIComboBox();
    p_shadowTypeInput->AddItem("None", SCAST<int>( Light::ShadowType::NONE ) );
    p_shadowTypeInput->AddItem("Hard", SCAST<int>( Light::ShadowType::HARD ) );
    p_shadowTypeInput->AddItem("Soft", SCAST<int>( Light::ShadowType::SOFT ) );
    p_shadowTypeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Shadow type", p_shadowTypeInput->GetGameObject());

    p_shadowMapSizeInput = GameObjectFactory::CreateUISlider();
    p_shadowMapSizeInput->SetMinMaxValues(1.0f, 4096);
    p_shadowMapSizeInput->GetInputNumber()->SetDecimalPlaces(0);
    p_shadowMapSizeInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Shadow map size", p_shadowMapSizeInput->GetGameObject());

    SetLabelsWidth(100);
}

void CIWLight::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    if (!p_intensityInput->HasFocus())
    {
        p_intensityInput->SetValue( GetLight()->GetIntensity() );
    }

    if (!p_colorInput->HasFocus())
    {
        p_colorInput->SetColor( GetLight()->GetColor() );
    }

    if (!p_shadowBiasInput->HasFocus())
    {
        p_shadowBiasInput->SetValue( GetLight()->GetShadowBias() );
    }

    if (!p_shadowTypeInput->HasFocus())
    {
        p_shadowTypeInput->SetSelectionByValue(
                    SCAST<int>(GetLight()->GetShadowType()) );
    }

    if (!p_shadowMapSizeInput->HasFocus())
    {
        p_shadowMapSizeInput->SetValue( GetLight()->GetShadowMapSize().x );
    }
}

Light *CIWLight::GetLight() const
{
    return SCAST<Light*>( GetComponent() );
}

void CIWLight::OnValueChanged(Object *object)
{
    ComponentInspectorWidget::OnValueChanged(object);

    GetLight()->SetIntensity( p_intensityInput->GetValue() );
    GetLight()->SetColor( p_colorInput->GetColor() );
    GetLight()->SetShadowBias( p_shadowBiasInput->GetValue() );
    GetLight()->SetShadowType( SCAST<Light::ShadowType>(
                                   p_shadowTypeInput->GetSelectedValue()) );
    GetLight()->SetShadowMapSize( Vector2i(p_shadowMapSizeInput->GetValue()) );
}
