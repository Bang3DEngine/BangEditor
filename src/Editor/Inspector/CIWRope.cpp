#include "BangEditor/CIWRope.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWRope::InitInnerWidgets()
{
    CIWRenderer::InitInnerWidgets();

    SetName("CIWRope");
    SetTitle("Rope");
}

void CIWRope::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();
}

void CIWRope::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);
}

CIWRope::CIWRope()
{
}

CIWRope::~CIWRope()
{
}

