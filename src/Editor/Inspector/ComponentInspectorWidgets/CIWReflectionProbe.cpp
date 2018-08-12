#include "BangEditor/CIWReflectionProbe.h"

#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/RectTransform.h"
#include "Bang/ReflectionProbe.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UITextureCubeMapPreviewer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWReflectionProbe::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWReflectionProbe");
    SetTitle("Reflection Probe");

    p_previewCMRenderer = GameObject::Create<UITextureCubeMapPreviewer>();

    AddLabel("Preview");
    AddWidget(p_previewCMRenderer, 200);

    SetLabelsWidth(60);
}

void CIWReflectionProbe::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    p_previewCMRenderer->SetTextureCubeMap( GetReflectionProbe()->
                                            GetTextureCubeMap() );
}

CIWReflectionProbe::CIWReflectionProbe()
{
}

CIWReflectionProbe::~CIWReflectionProbe()
{
}

ReflectionProbe *CIWReflectionProbe::GetReflectionProbe() const
{
    return SCAST<ReflectionProbe*>( GetComponent() );
}

void CIWReflectionProbe::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);
}

