#include "BangEditor/CIWCapsuleCollider.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Axis.h"
#include "Bang/CapsuleCollider.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

CIWCapsuleCollider::CIWCapsuleCollider()
{
}

CIWCapsuleCollider::~CIWCapsuleCollider()
{
}

void CIWCapsuleCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWCapsuleCollider");
    SetTitle("CapsuleCollider");

    p_radiusInput = GameObjectFactory::CreateUIInputNumber();
    p_radiusInput->SetMinValue(0.0f);
    p_radiusInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_heightInput = GameObjectFactory::CreateUIInputNumber();
    p_heightInput->SetMinValue(0.0f);
    p_heightInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_axisInput = GameObjectFactory::CreateUIComboBox();
    p_axisInput->AddItem("X", SCAST<int>(Axis3D::X));
    p_axisInput->AddItem("Y", SCAST<int>(Axis3D::Y));
    p_axisInput->AddItem("Z", SCAST<int>(Axis3D::Z));
    p_axisInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Radius", p_radiusInput->GetGameObject());
    AddWidget("Height", p_heightInput->GetGameObject());
    AddWidget("Axis", p_axisInput->GetGameObject());

    SetLabelsWidth(95);
}

void CIWCapsuleCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    CapsuleCollider *cc = GetCapsuleCollider();
    if (!p_radiusInput->HasFocus())
    {
        p_radiusInput->SetValue(cc->GetRadius());
    }
    if (!p_heightInput->HasFocus())
    {
        p_heightInput->SetValue(cc->GetHeight());
    }
    if (!p_axisInput->HasFocus())
    {
        p_axisInput->SetSelectionByValue(SCAST<int>(cc->GetAxis()));
    }
}

void CIWCapsuleCollider::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    CapsuleCollider *capsuleCollider = GetCapsuleCollider();
    capsuleCollider->SetRadius(p_radiusInput->GetValue());
    capsuleCollider->SetHeight(p_heightInput->GetValue());
    capsuleCollider->SetAxis(SCAST<Axis3D>(p_axisInput->GetSelectedValue()));
}

CapsuleCollider *CIWCapsuleCollider::GetCapsuleCollider() const
{
    return SCAST<CapsuleCollider *>(GetCollider());
}
