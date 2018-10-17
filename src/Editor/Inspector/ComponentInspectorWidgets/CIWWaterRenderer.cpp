#include "BangEditor/CIWWaterRenderer.h"

#include "Bang/WaterRenderer.h"

namespace Bang
{
class IEventsValueChanged;
template <class>
class EventEmitter;
}

using namespace Bang;
using namespace BangEditor;

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
    return SCAST<WaterRenderer *>(GetComponent());
}

void CIWWaterRenderer::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);
}
