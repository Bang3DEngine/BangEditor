#include "BangEditor/CIWRectTransform.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/RectTransform.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWRectTransform::InitInnerWidgets()
{
    CIWTransform::InitInnerWidgets();

    SetName("CIWRectTransform");
    SetTitle("Rect Transform");
}

void CIWRectTransform::UpdateFromReference()
{
    CIWTransform::UpdateFromReference();
}

bool CIWRectTransform::CanBeRemovedFromContextMenu() const
{
    return false;
}

RectTransform *CIWRectTransform::GetRectTransform() const
{
    return SCAST<RectTransform *>(GetComponent());
}

void CIWRectTransform::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWTransform::OnValueChangedCIW(object);
}

bool CIWRectTransform::MustShowEnabledCheckbox() const
{
    return false;
}
