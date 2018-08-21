#include "BangEditor/CIWCollider.h"

#include "Bang/Collider.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UIInputNumber.h"

#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWCollider::CIWCollider()
{
}

CIWCollider::~CIWCollider()
{
}

void CIWCollider::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWCollider");
    SetTitle("Collider");

    p_centerInput = GameObject::Create<UIInputVector>();
    p_centerInput->SetSize(3);
    p_centerInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Center", p_centerInput);

    SetLabelsWidth(75);
}

void CIWCollider::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Collider *collider = GetCollider();
    if (!p_centerInput->HasFocus())
    {
        p_centerInput->Set( collider->GetCenter() );
    }
}

void CIWCollider::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    Collider *collider = GetCollider();
    collider->SetCenter( p_centerInput->GetVector3() );
}

Collider *CIWCollider::GetCollider() const
{
    return SCAST<Collider*>( GetComponent() );
}

