#include "BangEditor/CIWReflectionProbe.h"

#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/UICheckBox.h"
#include "Bang/RectTransform.h"
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

    p_sizeInput = GameObject::Create<UIInputVector>();
    p_sizeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_sizeInput->SetSize(3);

    p_previewCMRenderer = GameObject::Create<UITextureCubeMapPreviewer>();

    AddWidget("Boxed", p_isBoxedCheckBox->GetGameObject());
    AddWidget("Size",  p_sizeInput);
    AddLabel("Preview");
    AddWidget(p_previewCMRenderer, 200);

    SetLabelsWidth(60);
}

void CIWReflectionProbe::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    ReflectionProbe *reflProbe = GetReflectionProbe();

    p_sizeInput->Set( reflProbe->GetSize() );
    p_isBoxedCheckBox->SetChecked( reflProbe->GetIsBoxed() );
    p_previewCMRenderer->SetTextureCubeMap( reflProbe->GetTextureCubeMap() );
}

void CIWReflectionProbe::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    ReflectionProbe *reflProbe = GetReflectionProbe();
    if (object == p_sizeInput)
    {
        reflProbe->SetSize( p_sizeInput->GetVector3() );
    }
    else if (object == p_isBoxedCheckBox)
    {
        reflProbe->SetIsBoxed( p_isBoxedCheckBox->IsChecked() );
    }
}

ReflectionProbe *CIWReflectionProbe::GetReflectionProbe() const
{
    return SCAST<ReflectionProbe*>( GetComponent() );
}
