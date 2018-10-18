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

    p_bouncinessInput = GameObjectFactory::CreateUISlider();
    p_bouncinessInput->SetMinMaxValues(0.0f, 1.0f);
    p_bouncinessInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_dampingInput = GameObjectFactory::CreateUISlider();
    p_dampingInput->SetMinMaxValues(0.0f, 1.0f);
    p_dampingInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_ropeLengthInput = GameObjectFactory::CreateUIInputNumber();
    p_ropeLengthInput->SetMinValue(0.05f);
    p_ropeLengthInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_springsForceInput = GameObjectFactory::CreateUIInputNumber();
    p_springsForceInput->SetMinValue(0.0f);
    p_springsForceInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_seeDebugPointsInput = GameObjectFactory::CreateUICheckBox();
    p_seeDebugPointsInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    AddWidget("Bounciness", p_bouncinessInput->GetGameObject());
    AddWidget("Damping", p_dampingInput->GetGameObject());
    AddWidget("Rope Length", p_ropeLengthInput->GetGameObject());
    AddWidget("Springs force", p_springsForceInput->GetGameObject());
    AddWidget("See points", p_seeDebugPointsInput->GetGameObject());

    SetLabelsWidth(95);
}

void CIWRope::UpdateFromReference()
{
    CIWRenderer::UpdateFromReference();

    p_bouncinessInput->SetValue(GetRope()->GetBounciness());
    p_ropeLengthInput->SetValue(GetRope()->GetRopeLength());
    p_dampingInput->SetValue(GetRope()->GetDamping());
    p_springsForceInput->SetValue(GetRope()->GetSpringsForce());
    p_seeDebugPointsInput->SetChecked(GetRope()->GetSeeDebugPoints());
}

void CIWRope::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    CIWRenderer::OnValueChangedCIW(object);

    if (object == p_bouncinessInput)
    {
        GetRope()->SetBounciness(p_bouncinessInput->GetValue());
    }
    else if (object == p_dampingInput)
    {
        GetRope()->SetDamping(p_dampingInput->GetValue());
    }
    else if (object == p_springsForceInput)
    {
        GetRope()->SetSpringsForce(p_springsForceInput->GetValue());
    }
    else if (object == p_ropeLengthInput)
    {
        GetRope()->SetRopeLength(p_ropeLengthInput->GetValue());
    }
    else if (object == p_seeDebugPointsInput)
    {
        GetRope()->SetSeeDebugPoints(p_seeDebugPointsInput->IsChecked());
    }
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
