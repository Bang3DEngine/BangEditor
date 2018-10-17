#include "BangEditor/CIWSphereCollider.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/SphereCollider.h"
#include "Bang/UIInputNumber.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

CIWSphereCollider::CIWSphereCollider()
{
}

CIWSphereCollider::~CIWSphereCollider()
{
}

void CIWSphereCollider::InitInnerWidgets()
{
    CIWCollider::InitInnerWidgets();

    SetName("CIWSphereCollider");
    SetTitle("SphereCollider");

    p_radiusInput = GameObjectFactory::CreateUIInputNumber();
    p_radiusInput->SetMinValue(0.01f);
    p_radiusInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Radius", p_radiusInput->GetGameObject());

    SetLabelsWidth(95);
}

void CIWSphereCollider::UpdateFromReference()
{
    CIWCollider::UpdateFromReference();

    SphereCollider *sphereCollider = GetSphereCollider();
    if(!p_radiusInput->HasFocus())
    {
        p_radiusInput->SetValue(sphereCollider->GetRadius());
    }
}

void CIWSphereCollider::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    CIWCollider::OnValueChangedCIW(object);

    SphereCollider *sphereCollider = GetSphereCollider();
    sphereCollider->SetRadius(p_radiusInput->GetValue());
}

SphereCollider *CIWSphereCollider::GetSphereCollider() const
{
    return SCAST<SphereCollider *>(GetCollider());
}
