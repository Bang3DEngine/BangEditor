#include "BangEditor/RIWAudioClip.h"

#include "Bang/UIButton.h"
#include "Bang/Extensions.h"
#include "Bang/UIInputText.h"
#include "Bang/AudioManager.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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

    AddWidget("Duration", p_duration->GetGameObject());
    AddWidget("Frequency", p_frequency->GetGameObject());
    AddWidget("Bit depth", p_bitDepth->GetGameObject());
    AddWidget("Buffer size", p_bufferSize->GetGameObject());
    AddWidget("Num channels", p_numChannels->GetGameObject());

    SetLabelsWidth(130);
}

void RIWAudioClip::UpdateInputsFromResource()
{
    p_duration->GetText()->SetContent(
                String::ToString(GetAudioClip()->GetLength()) + "s.");
    p_frequency->GetText()->SetContent(
                String::ToString(GetAudioClip()->GetFrequency()) + " hz.");
    p_bitDepth->SetValue( GetAudioClip()->GetBitDepth() );
    p_bufferSize->SetValue( GetAudioClip()->GetBufferSize() );
    p_numChannels->SetValue( GetAudioClip()->GetChannels() );
}

Texture2D *RIWAudioClip::GetIconTexture() const
{
    return EditorTextureFactory::GetIconForExtension(
                        Extensions::GetAudioClipExtensions().Front());
}

void RIWAudioClip::OnValueChangedRIWResource(EventEmitter<IEventsValueChanged> *object)
{
    // Empty
}

AudioClip *RIWAudioClip::GetAudioClip() const
{
    return SCAST<AudioClip*>( GetResource().Get() );
}


