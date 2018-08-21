#include "BangEditor/CIWRigidBody.h"

#include "Bang/RigidBody.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFile.h"
#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWRigidBody::CIWRigidBody()
{
}

CIWRigidBody::~CIWRigidBody()
{
}

void CIWRigidBody::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWRigidBody");
    SetTitle("RigidBody");

    p_massInput = GameObjectFactory::CreateUIInputNumber();
    p_massInput->SetMinMaxValues(0.0f, Math::Infinity<float>());
    p_massInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_dragInput = GameObjectFactory::CreateUIInputNumber();
    p_dragInput->SetMinMaxValues(0.0f, 1.0f);
    p_dragInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_angularDragInput = GameObjectFactory::CreateUIInputNumber();
    p_angularDragInput->SetMinMaxValues(0.0f, 1.0f);
    p_angularDragInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_useGravityInput = GameObjectFactory::CreateUICheckBox();
    p_useGravityInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_isKinematicInput = GameObjectFactory::CreateUICheckBox();
    p_isKinematicInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Mass", p_massInput->GetGameObject());
    AddWidget("Drag", p_dragInput->GetGameObject());
    AddWidget("Angular Drag", p_angularDragInput->GetGameObject());
    AddWidget("Use Gravity",  p_useGravityInput->GetGameObject());
    AddWidget("Is Kinematic", p_isKinematicInput->GetGameObject());

    SetLabelsWidth(100);
}

void CIWRigidBody::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    RigidBody *rigidBody = GetRigidBody();

    p_massInput->SetValue( rigidBody->GetMass() );
    p_dragInput->SetValue( rigidBody->GetDrag() );
    p_angularDragInput->SetValue( rigidBody->GetAngularDrag() );
    p_useGravityInput->SetChecked( rigidBody->GetUseGravity() );
    p_isKinematicInput->SetChecked( rigidBody->GetIsKinematic() );
}

void CIWRigidBody::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    RigidBody *rigidBody = GetRigidBody();

    if (object == p_massInput)
    {
        rigidBody->SetMass( p_massInput->GetValue() );
    }
    else if (object == p_dragInput)
    {
        rigidBody->SetDrag( p_dragInput->GetValue() );
    }
    else if (object == p_angularDragInput)
    {
        rigidBody->SetAngularDrag( p_angularDragInput->GetValue() );
    }
    else if (object == p_useGravityInput)
    {
        rigidBody->SetUseGravity( p_useGravityInput->IsChecked() );
    }
    else if (object == p_isKinematicInput)
    {
        rigidBody->SetIsKinematic( p_isKinematicInput->IsChecked() );
    }
}

RigidBody *CIWRigidBody::GetRigidBody() const
{
    return SCAST<RigidBody*>( GetComponent() );
}


