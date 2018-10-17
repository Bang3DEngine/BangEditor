#include "BangEditor/CIWRigidBody.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Math.h"
#include "Bang/RigidBody.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

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
    p_angularDragInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_useGravityInput = GameObjectFactory::CreateUICheckBox();
    p_useGravityInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_isKinematicInput = GameObjectFactory::CreateUICheckBox();
    p_isKinematicInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_constraintsInput = GameObjectFactory::CreateUIBoolComboBox();
    p_constraintsInput->AddItem(
        "Lock Position X", SCAST<int>(RigidBodyConstraint::LOCK_POSITION_X));
    p_constraintsInput->AddItem(
        "Lock Position Y", SCAST<int>(RigidBodyConstraint::LOCK_POSITION_Y));
    p_constraintsInput->AddItem(
        "Lock Position Z", SCAST<int>(RigidBodyConstraint::LOCK_POSITION_Z));
    p_constraintsInput->AddItem(
        "Lock Rotation X", SCAST<int>(RigidBodyConstraint::LOCK_ROTATION_X));
    p_constraintsInput->AddItem(
        "Lock Rotation Y", SCAST<int>(RigidBodyConstraint::LOCK_ROTATION_Y));
    p_constraintsInput->AddItem(
        "Lock Rotation Z", SCAST<int>(RigidBodyConstraint::LOCK_ROTATION_Z));
    p_constraintsInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    AddWidget("Mass", p_massInput->GetGameObject());
    AddWidget("Drag", p_dragInput->GetGameObject());
    AddWidget("Angular Drag", p_angularDragInput->GetGameObject());
    AddWidget("Use Gravity", p_useGravityInput->GetGameObject());
    AddWidget("Is Kinematic", p_isKinematicInput->GetGameObject());
    AddWidget("Constraints", p_constraintsInput->GetGameObject());

    SetLabelsWidth(100);
}

void CIWRigidBody::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    RigidBody *rigidBody = GetRigidBody();

    p_massInput->SetValue(rigidBody->GetMass());
    p_dragInput->SetValue(rigidBody->GetDrag());
    p_angularDragInput->SetValue(rigidBody->GetAngularDrag());
    p_useGravityInput->SetChecked(rigidBody->GetUseGravity());
    p_isKinematicInput->SetChecked(rigidBody->GetIsKinematic());
    p_constraintsInput->SetSelectionForFlag(
        SCAST<int>(rigidBody->GetConstraints().GetValue()));
}

void CIWRigidBody::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    RigidBody *rigidBody = GetRigidBody();

    if (object == p_massInput)
    {
        rigidBody->SetMass(p_massInput->GetValue());
    }
    else if (object == p_dragInput)
    {
        rigidBody->SetDrag(p_dragInput->GetValue());
    }
    else if (object == p_angularDragInput)
    {
        rigidBody->SetAngularDrag(p_angularDragInput->GetValue());
    }
    else if (object == p_useGravityInput)
    {
        rigidBody->SetUseGravity(p_useGravityInput->IsChecked());
    }
    else if (object == p_isKinematicInput)
    {
        rigidBody->SetIsKinematic(p_isKinematicInput->IsChecked());
    }
    else if (object == p_constraintsInput)
    {
        rigidBody->SetConstraints(SCAST<RigidBodyConstraint>(
            p_constraintsInput->GetSelectedValuesForFlag()));
    }
}

RigidBody *CIWRigidBody::GetRigidBody() const
{
    return SCAST<RigidBody *>(GetComponent());
}
