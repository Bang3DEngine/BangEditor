#include "BangEditor/CIWRope.h"

#include <sys/types.h>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Rope.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWRope::InitInnerWidgets()
{
    CIWRenderer::InitInnerWidgets();

    SetName("CIWRope");
    SetTitle("Rope");

    SetLabelsWidth(95);
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

Rope *CIWRope::GetRope() const
{
    return SCAST<Rope *>(GetComponent());
}
