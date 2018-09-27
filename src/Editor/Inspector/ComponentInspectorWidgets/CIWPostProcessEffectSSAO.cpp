#include "BangEditor/CIWPostProcessEffectSSAO.h"

#include "Bang/UISlider.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/PostProcessEffectSSAO.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWPostProcessEffectSSAO::CIWPostProcessEffectSSAO()
{
}

CIWPostProcessEffectSSAO::~CIWPostProcessEffectSSAO()
{
}

void CIWPostProcessEffectSSAO::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWPostProcessEffectSSAO");
    SetTitle("Post Process Effect SSAO");

    p_intensityInput = GameObjectFactory::CreateUISlider();
    p_intensityInput->SetMinMaxValues(0.0f, 10.0f);
    p_intensityInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_radiusInput = GameObjectFactory::CreateUIInputNumber();
    p_radiusInput->SetMinValue(0.001f);
    p_radiusInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_blurRadiusInput = GameObjectFactory::CreateUISlider();
    p_blurRadiusInput->SetMinMaxValues(0.0f, 10.0f);
    p_blurRadiusInput->GetInputNumber()->SetDecimalPlaces(0);
    p_blurRadiusInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_numRandAxesInput = GameObjectFactory::CreateUISlider();
    p_numRandAxesInput->SetMinMaxValues(1.0f, 128.0f);
    p_numRandAxesInput->GetInputNumber()->SetDecimalPlaces(0);
    p_numRandAxesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_numRandSamplesInput = GameObjectFactory::CreateUISlider();
    p_numRandSamplesInput->SetMinMaxValues(1.0f, 128.0f);
    p_numRandSamplesInput->GetInputNumber()->SetDecimalPlaces(0);
    p_numRandSamplesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_bilateralBlurInput = GameObjectFactory::CreateUICheckBox();
    p_bilateralBlurInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_separableInput = GameObjectFactory::CreateUICheckBox();
    p_separableInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Intensity", p_intensityInput->GetGameObject());
    AddWidget("Radius", p_radiusInput->GetGameObject());
    AddWidget("Blur Radius", p_blurRadiusInput->GetGameObject());
    AddWidget("Num samples", p_numRandSamplesInput->GetGameObject());
    AddWidget("Num random axes", p_numRandAxesInput->GetGameObject());
    AddWidget("Bilateral Blur", p_bilateralBlurInput->GetGameObject());
    AddWidget("Separable", p_separableInput->GetGameObject());

    SetLabelsWidth(110);
}

void CIWPostProcessEffectSSAO::UpdateFromReference()
{
    PostProcessEffectSSAO *ppe = GetPostProcessEffectSSAO();

    p_intensityInput->SetValue( ppe->GetSSAOIntensity() );
    p_radiusInput->SetValue( ppe->GetSSAORadius() );
    p_blurRadiusInput->SetValue( ppe->GetBlurRadius() );
    p_numRandAxesInput->SetValue( ppe->GetNumRandomAxes() );
    p_numRandSamplesInput->SetValue( ppe->GetNumRandomSamples() );
    p_bilateralBlurInput->SetChecked( ppe->GetBilateralBlurEnabled() );
    p_separableInput->SetChecked( ppe->GetSeparable() );
}

void CIWPostProcessEffectSSAO::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    PostProcessEffectSSAO *ppe = GetPostProcessEffectSSAO();

    ppe->SetSSAOIntensity( p_intensityInput->GetValue() );
    ppe->SetSSAORadius( p_radiusInput->GetValue() );
    ppe->SetBlurRadius( p_blurRadiusInput->GetValue() );
    ppe->SetNumRandomAxes( p_numRandAxesInput->GetValue() );
    ppe->SetNumRandomSamples( p_numRandSamplesInput->GetValue() );
    ppe->SetBilateralBlurEnabled( p_bilateralBlurInput->IsChecked() );
    ppe->SetSeparable( p_separableInput->IsChecked() );
}

PostProcessEffectSSAO *CIWPostProcessEffectSSAO::GetPostProcessEffectSSAO() const
{
    return SCAST<PostProcessEffectSSAO*>( GetComponent() );
}

