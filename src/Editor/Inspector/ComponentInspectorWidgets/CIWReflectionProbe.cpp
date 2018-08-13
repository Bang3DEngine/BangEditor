#include "BangEditor/CIWReflectionProbe.h"

#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/UICheckBox.h"
#include "Bang/RectTransform.h"
#include "Bang/UIInputNumber.h"
#include "Bang/ReflectionProbe.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputVector.h"
#include "BangEditor/UITextureCubeMapPreviewer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWReflectionProbe::CIWReflectionProbe()
{
}

CIWReflectionProbe::~CIWReflectionProbe()
{
}

void CIWReflectionProbe::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWReflectionProbe");
    SetTitle("Reflection Probe");

    p_isBoxedCheckBox = GameObjectFactory::CreateUICheckBox();
    p_isBoxedCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_filterForIBLCheckBox = GameObjectFactory::CreateUICheckBox();
    p_filterForIBLCheckBox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_sizeInput = GameObject::Create<UIInputVector>();
    p_sizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sizeInput->GetInputNumbers()[0]->SetMinMaxValues(0.0f, Math::Infinity<float>());
    p_sizeInput->GetInputNumbers()[1]->SetMinMaxValues(0.0f, Math::Infinity<float>());
    p_sizeInput->GetInputNumbers()[2]->SetMinMaxValues(0.0f, Math::Infinity<float>());
    p_sizeInput->SetSize(3);

    p_restTimeInput = GameObjectFactory::CreateUIInputNumber();
    p_restTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_restTimeInput->SetMinMaxValues(0.0f, Math::Infinity<float>());
    p_restTimeInput->SetDecimalPlaces(1);

    p_previewCMRenderer = GameObject::Create<UITextureCubeMapPreviewer>();

    AddWidget("Rest time sec.", p_restTimeInput->GetGameObject());
    AddWidget("Boxed", p_isBoxedCheckBox->GetGameObject());
    AddWidget("Filter for IBL", p_filterForIBLCheckBox->GetGameObject());
    AddWidget("Size",  p_sizeInput);
    AddLabel("Preview");
    AddWidget(p_previewCMRenderer, 200);

    SetLabelsWidth(100);
}

void CIWReflectionProbe::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    ReflectionProbe *reflProbe = GetReflectionProbe();

    p_sizeInput->Set( reflProbe->GetSize() );
    p_restTimeInput->SetValue( reflProbe->GetRestTimeSeconds() );
    p_isBoxedCheckBox->SetChecked( reflProbe->GetIsBoxed() );
    p_filterForIBLCheckBox->SetChecked( reflProbe->GetFilterForIBL() );
    p_previewCMRenderer->SetTextureCubeMap( reflProbe->
                                            GetTextureCubeMapWithoutFiltering() );
}

void CIWReflectionProbe::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    ReflectionProbe *reflProbe = GetReflectionProbe();
    reflProbe->SetSize( p_sizeInput->GetVector3() );
    reflProbe->SetIsBoxed( p_isBoxedCheckBox->IsChecked() );
    reflProbe->SetRestTimeSeconds( p_restTimeInput->GetValue() );
    reflProbe->SetFilterForIBL( p_filterForIBLCheckBox->IsChecked() );
}

ReflectionProbe *CIWReflectionProbe::GetReflectionProbe() const
{
    return SCAST<ReflectionProbe*>( GetComponent() );
}
