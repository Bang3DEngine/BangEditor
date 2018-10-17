#include "BangEditor/CIWPointLight.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/PointLight.h"
#include "Bang/UIInputNumber.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWPointLight::InitInnerWidgets()
{
    CIWLight::InitInnerWidgets();

    SetName("CIWPointLight");
    SetTitle("Point Light");

    p_rangeInput = GameObjectFactory::CreateUIInputNumber();
    p_rangeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_rangeInput->SetMinValue(0.0f);
    AddWidget("Range", p_rangeInput->GetGameObject());
}

void CIWPointLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();

    if (!p_rangeInput->HasFocus())
    {
        p_rangeInput->SetValue(GetPointLight()->GetRange());
    }
}

PointLight *CIWPointLight::GetPointLight() const
{
    return SCAST<PointLight *>(GetLight());
}

void CIWPointLight::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWLight::OnValueChangedCIW(object);

    GetPointLight()->SetRange(p_rangeInput->GetValue());
}
