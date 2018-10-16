#include "BangEditor/CIWWaterRenderer.h"

#include "Bang/WaterRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWWaterRenderer::InitInnerWidgets()
{
    CIWRenderer::InitInnerWidgets();

    SetName("CIWWaterRenderer");
    SetTitle("Water Renderer");
}

void CIWWaterRenderer::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();
}

CIWWaterRenderer::CIWWaterRenderer()
{
}

CIWWaterRenderer::~CIWWaterRenderer()
{
}

WaterRenderer *CIWWaterRenderer::GetWaterRenderer() const
{
    return SCAST<WaterRenderer*>( GetComponent() );
}

void CIWWaterRenderer::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);
}

