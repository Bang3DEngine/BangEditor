#include "BangEditor/CIWDirectionalLight.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/DirectionalLight.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/UIInputNumber.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWDirectionalLight::InitInnerWidgets()
{
    CIWLight::InitInnerWidgets();

    SetName("CIWDirectionalLight");
    SetTitle("Directional Light");

    p_shadowDistanceInput = GameObjectFactory::CreateUIInputNumber();
    p_shadowDistanceInput->SetMinMaxValues(1.0f, 2000.0f);
    p_shadowDistanceInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);
    AddWidget("Shadow dist.", p_shadowDistanceInput->GetGameObject());

    SetLabelsWidth(110);
}

void CIWDirectionalLight::UpdateFromReference()
{
    CIWLight::UpdateFromReference();

    if(!p_shadowDistanceInput->HasFocus())
    {
        p_shadowDistanceInput->SetValue(
            GetDirectionalLight()->GetShadowDistance());
    }
}

DirectionalLight *CIWDirectionalLight::GetDirectionalLight() const
{
    return SCAST<DirectionalLight *>(GetLight());
}

void CIWDirectionalLight::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWLight::OnValueChangedCIW(object);

    GetDirectionalLight()->SetShadowDistance(p_shadowDistanceInput->GetValue());
}
