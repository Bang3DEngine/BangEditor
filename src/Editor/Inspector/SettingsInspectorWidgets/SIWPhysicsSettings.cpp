#include "BangEditor/SIWPhysicsSettings.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/Physics.h"
#include "Bang/Time.h"
#include "Bang/UIInputNumber.h"
#include "BangEditor/ProjectManager.h"
#include "BangEditor/UIInputVector.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SIWPhysicsSettings::SIWPhysicsSettings()
{
}

SIWPhysicsSettings::~SIWPhysicsSettings()
{
}

void SIWPhysicsSettings::InitInnerWidgets()
{
    SettingsInspectorWidget::InitInnerWidgets();

    SetTitle("Physics Settings");
    SetName("SIWPhysicsSettings");

    p_stepSleepTimeInput = GameObjectFactory::CreateUIInputNumber();
    p_stepSleepTimeInput->SetDecimalPlaces(4);
    p_stepSleepTimeInput->SetMinValue(0.001f);
    p_stepSleepTimeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_maxSubStepsInput = GameObjectFactory::CreateUIInputNumber();
    p_maxSubStepsInput->SetDecimalPlaces(0);
    p_maxSubStepsInput->SetMinValue(1.0f);
    p_maxSubStepsInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_gravityInput = GameObject::Create<UIInputVector>();
    p_gravityInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Step sleep time (s)", p_stepSleepTimeInput->GetGameObject());
    AddWidget("Max sub steps", p_maxSubStepsInput->GetGameObject());
    AddWidget("Gravity", p_gravityInput);
    SetLabelsWidth(110);

    Update();
}

void SIWPhysicsSettings::UpdateFromReference()
{
    SettingsInspectorWidget::UpdateFromReference();

    if (!p_stepSleepTimeInput->HasFocus())
    {
        p_stepSleepTimeInput->SetValue(Physics::GetInstance()->
                                       GetStepSleepTime().GetSeconds());
    }

    if (!p_maxSubStepsInput->HasFocus())
    {
        p_maxSubStepsInput->SetValue(Physics::GetInstance()->GetMaxSubSteps());
    }

    if (!p_gravityInput->HasFocus())
    {
        p_gravityInput->Set(Physics::GetInstance()->GetGravity());
    }
}

void SIWPhysicsSettings::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    Physics::GetInstance()->SetStepSleepTime(
                    Time::Seconds(p_stepSleepTimeInput->GetValue()) );
    Physics::GetInstance()->SetMaxSubSteps( p_maxSubStepsInput->GetValue() );
    Physics::GetInstance()->SetGravity( p_gravityInput->GetVector3() );

    ProjectManager::ExportCurrentProject();
}
