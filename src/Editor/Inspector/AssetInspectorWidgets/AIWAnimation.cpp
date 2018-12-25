#include "BangEditor/AIWAnimation.h"

#include "Bang/AssetHandle.h"
#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Math.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/AIWAsset.tcc"
#include "BangEditor/EditorTextureFactory.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

AIWAnimation::AIWAnimation()
{
}

AIWAnimation::~AIWAnimation()
{
}

void AIWAnimation::Init()
{
    AIWAsset<Animation>::Init();

    SetName("AIWAnimation");
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

Animation *AIWAnimation::GetAnimation() const
{
    return SCAST<Animation *>(GetAsset().Get());
}

void AIWAnimation::UpdateInputsFromAsset()
{
    p_speedInput->SetValue(GetAnimation()->GetSpeed());
    p_wrapModeInput->SetSelectionByValue(
        SCAST<int>(GetAnimation()->GetWrapMode()));

    float durationSecs = GetAnimation()->GetDurationInFrames() /
                         Math::Max(1.0f, GetAnimation()->GetFramesPerSecond());
    p_durationInSeconds->GetText()->SetContent(String::ToString(durationSecs) +
                                               " s.");
}

Texture2D *AIWAnimation::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
        Extensions::GetAnimationExtension());
}

void AIWAnimation::OnValueChangedAIWAsset(
    EventEmitter<IEventsValueChanged> *object)
{
    BANG_UNUSED(object);
    GetAnimation()->SetSpeed(p_speedInput->GetValue());
    GetAnimation()->SetWrapMode(
        SCAST<AnimationWrapMode>(p_wrapModeInput->GetSelectedValue()));
}
