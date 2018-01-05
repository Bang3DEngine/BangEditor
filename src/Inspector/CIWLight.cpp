#include "BangEditor/CIWLight.h"

#include "Bang/Light.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputColor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWLight::Init()
{
    ComponentInspectorWidget::Init();

    SetName("CIWLight");
    SetTitle("Light");

    p_intensityInput = GameObjectFactory::CreateUIInputNumber();
    p_intensityInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Intensity", p_intensityInput->GetGameObject());

    p_colorInput = GameObject::Create<UIInputColor>();
    p_colorInput->EventEmitter<IValueChangedListener>::RegisterListener(this);
    AddWidget("Color", p_colorInput);

    SetLabelsWidth(70);
}

void CIWLight::UpdateValuesFromComponent()
{
    ComponentInspectorWidget::UpdateValuesFromComponent();

    if (!p_intensityInput->HasFocus())
    {
        p_intensityInput->SetValue( GetLight()->GetIntensity() );
    }

    if (!p_colorInput->HasFocus())
    {
        p_colorInput->SetColor( GetLight()->GetColor() );
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
}
