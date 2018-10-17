#include "BangEditor/CIWBoxCollider.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BoxCollider.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/UIInputNumber.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

CIWBoxCollider::CIWBoxCollider()
{
}

CIWBoxCollider::~CIWBoxCollider()
{
}

void CIWBoxCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWBoxCollider");
    SetTitle("BoxCollider");

    p_extentsInput = GameObject::Create<UIInputVector>();
    for(UIInputNumber *inputNumber : p_extentsInput->GetInputNumbers())
    {
        inputNumber->SetMinValue(0.01f);
    }
    p_extentsInput->SetSize(3);
    p_extentsInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Extents", p_extentsInput);

    SetLabelsWidth(95);
}

void CIWBoxCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    BoxCollider *boxCollider = GetBoxCollider();
    if(!p_extentsInput->HasFocus())
    {
        p_extentsInput->Set(boxCollider->GetExtents());
    }
}

void CIWBoxCollider::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    BoxCollider *boxCollider = GetBoxCollider();
    boxCollider->SetExtents(p_extentsInput->GetVector3());
}

BoxCollider *CIWBoxCollider::GetBoxCollider() const
{
    return SCAST<BoxCollider *>(GetCollider());
}
