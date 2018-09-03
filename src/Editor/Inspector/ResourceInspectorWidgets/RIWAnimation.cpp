#include "BangEditor/RIWAnimation.h"

#include "Bang/UISlider.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RIWAnimation::RIWAnimation()
{
}

RIWAnimation::~RIWAnimation()
{
}

void RIWAnimation::Init()
{
    RIWResource<Animation>::Init();

    SetName("RIWAnimation");
    SetTitle("Animation");

    p_speedInput = GameObjectFactory::CreateUIInputNumber();
    p_speedInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_wrapModeInput = GameObjectFactory::CreateUIComboBox();
    p_wrapModeInput->AddItem("Clamp", SCAST<int>(AnimationWrapMode::CLAMP));
    p_wrapModeInput->AddItem("Repeat", SCAST<int>(AnimationWrapMode::REPEAT));
    p_wrapModeInput->AddItem("PingPong", SCAST<int>(AnimationWrapMode::PING_PONG));
    p_wrapModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Speed", p_speedInput->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeInput->GetGameObject());
    SetLabelsWidth(130);
}

Animation *RIWAnimation::GetAnimation() const
{
    return SCAST<Animation*>(GetResource().Get());
}

void RIWAnimation::UpdateInputsFromResource()
{
    p_speedInput->SetValue( GetAnimation()->GetSpeed() );
    p_wrapModeInput->SetSelectionByValue(
                            SCAST<int>(GetAnimation()->GetWrapMode()) );
}

void RIWAnimation::OnValueChangedRIWResource(
                            EventEmitter<IEventsValueChanged> *object)
{
    GetAnimation()->SetSpeed( p_speedInput->GetValue() );
    GetAnimation()->SetWrapMode(
                SCAST<AnimationWrapMode>(p_wrapModeInput->GetSelectedValue()) );
}

