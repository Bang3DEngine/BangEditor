#include "BangEditor/RIWAudioClip.h"

#include "Bang/ALAudioSource.h"
#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/AudioManager.h"
#include "Bang/AudioParams.h"
#include "Bang/EventEmitter.h"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ResourceHandle.h"
#include "Bang/UIButton.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/RIWResource.tcc"

namespace Bang
{
class IEventsDestroy;
class IEventsValueChanged;
class Texture2D;
}

using namespace Bang;
using namespace BangEditor;

RIWAudioClip::RIWAudioClip()
{
}

RIWAudioClip::~RIWAudioClip()
{
}

void RIWAudioClip::Init()
{
    RIWResource<AudioClip>::Init();

    SetName("RIWAudioClip");
    SetTitle("Audio Clip");

    p_playStopButton = GameObjectFactory::CreateUIButton("Play");
    p_playStopButton->AddClickedCallback([this]() {
        if(p_playStopButton->GetText()->GetContent() == "Stop")
        {
            Stop();
        }
        else
        {
            Play();
        }
    });

    p_duration = GameObjectFactory::CreateUIInputText();
    p_duration->SetBlocked(true);

    p_frequency = GameObjectFactory::CreateUIInputText();
    p_frequency->SetBlocked(true);

    p_bitDepth = GameObjectFactory::CreateUIInputNumber();
    p_bitDepth->GetInputText()->SetBlocked(true);
    p_bitDepth->SetDecimalPlaces(0);

    p_bufferSize = GameObjectFactory::CreateUIInputNumber();
    p_bufferSize->GetInputText()->SetBlocked(true);
    p_bufferSize->SetDecimalPlaces(0);

    p_numChannels = GameObjectFactory::CreateUIInputNumber();
    p_numChannels->GetInputText()->SetBlocked(true);
    p_numChannels->SetDecimalPlaces(0);

    AddWidget(p_playStopButton->GetGameObject());
    AddWidget(
        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("Duration", p_duration->GetGameObject());
    AddWidget("Frequency", p_frequency->GetGameObject());
    AddWidget("Bit depth", p_bitDepth->GetGameObject());
    AddWidget("Buffer size", p_bufferSize->GetGameObject());
    AddWidget("Num channels", p_numChannels->GetGameObject());

    SetLabelsWidth(130);
}

void RIWAudioClip::Play()
{
    AudioParams params;
    p_alAudioSourceBeingPlayed = AudioManager::Play(GetAudioClip(), params);
    p_alAudioSourceBeingPlayed->EventEmitter<IEventsDestroy>::RegisterListener(
        this);
    p_playStopButton->GetText()->SetContent("Stop");
}

void RIWAudioClip::Stop()
{
    if(p_alAudioSourceBeingPlayed)
    {
        p_alAudioSourceBeingPlayed->Stop();
        p_alAudioSourceBeingPlayed
            ->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
    }
    p_playStopButton->GetText()->SetContent("Play");
}

void RIWAudioClip::UpdateInputsFromResource()
{
    p_duration->GetText()->SetContent(
        String::ToString(GetAudioClip()->GetLength()) + "s.");
    p_frequency->GetText()->SetContent(
        String::ToString(GetAudioClip()->GetFrequency()) + " hz.");
    p_bitDepth->SetValue(GetAudioClip()->GetBitDepth());
    p_bufferSize->SetValue(GetAudioClip()->GetBufferSize());
    p_numChannels->SetValue(GetAudioClip()->GetChannels());
}

Texture2D *RIWAudioClip::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
        Extensions::GetAudioClipExtensions().Front());
}

Color RIWAudioClip::GetIconTint() const
{
    return EditorTextureFactory::GetComponentIconTint("AudioSource");
}

void RIWAudioClip::OnValueChangedRIWResource(
    EventEmitter<IEventsValueChanged> *object)
{
    // Empty
    BANG_UNUSED(object);
}

void RIWAudioClip::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject::OnDestroyed(object);

    ASSERT(object == p_alAudioSourceBeingPlayed);
    p_alAudioSourceBeingPlayed = nullptr;
    Stop();
}

AudioClip *RIWAudioClip::GetAudioClip() const
{
    return SCAST<AudioClip *>(GetResource().Get());
}
