#include "BangEditor/RIWAnimation.h"

#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Math.h"
#include "Bang/ResourceHandle.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/RIWResource.tcc"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

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
    p_wrapModeInput->AddItem("PingPong",
                             SCAST<int>(AnimationWrapMode::PING_PONG));
    p_wrapModeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_durationInSeconds = GameObjectFactory::CreateUIInputText();
    p_durationInSeconds->SetBlocked(true);

    AddWidget("Speed", p_speedInput->GetGameObject());
    AddWidget("Wrap Mode", p_wrapModeInput->GetGameObject());
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Duration ", p_durationInSeconds->GetGameObject());

    SetLabelsWidth(130);
}

Animation *RIWAnimation::GetAnimation() const
{
    return SCAST<Animation *>(GetResource().Get());
}

void RIWAnimation::UpdateInputsFromResource()
{
    p_speedInput->SetValue(GetAnimation()->GetSpeed());
    p_wrapModeInput->SetSelectionByValue(
        SCAST<int>(GetAnimation()->GetWrapMode()));

    float durationSecs = GetAnimation()->GetDurationInFrames() /
                         Math::Max(1.0f, GetAnimation()->GetFramesPerSecond());
    p_durationInSeconds->GetText()->SetContent(String::ToString(durationSecs) +
                                               " s.");
}

Texture2D *RIWAnimation::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
        Extensions::GetAnimationExtension());
}

void RIWAnimation::OnValueChangedRIWResource(
    EventEmitter<IEventsValueChanged> *object)
{
    GetAnimation()->SetSpeed(p_speedInput->GetValue());
    GetAnimation()->SetWrapMode(
        SCAST<AnimationWrapMode>(p_wrapModeInput->GetSelectedValue()));
}
