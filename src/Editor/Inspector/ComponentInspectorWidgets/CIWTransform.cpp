#include "BangEditor/CIWTransform.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Transform.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWTransform::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWTransform");
    SetTitle("Transform");

    SetLabelsWidth(60);
}

void CIWTransform::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();
}

bool CIWTransform::CanBeRemovedFromContextMenu() const
{
    return false;
}

Transform *CIWTransform::GetTransform() const
{
    return SCAST<Transform *>(GetComponent());
}

void CIWTransform::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);
}

bool CIWTransform::MustShowEnabledCheckbox() const
{
    return false;
}
