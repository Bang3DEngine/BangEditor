#include "BangEditor/CIWAudioSource.h"

#include "Bang/UIButton.h"
#include "Bang/UISlider.h"
#include "Bang/AudioClip.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/AudioSource.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void CIWAudioSource::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWAudioSource");
    SetTitle("AudioSource");

    p_audioClipFileInput = GameObject::Create<UIInputFileWithPreview>();
    p_audioClipFileInput->SetExtensions( Extensions::GetAudioClipExtensions() );
    p_audioClipFileInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_audioClipFileInput->SetZoomable(false);

    p_rangeInput = GameObjectFactory::CreateUIInputNumber();
    p_rangeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    p_rangeInput->SetMinValue(0.0f);

    p_volumeSlider = GameObjectFactory::CreateUISlider();
    p_volumeSlider->SetMinMaxValues(0.0f, 5.0f);
    p_volumeSlider->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_pitchSlider = GameObjectFactory::CreateUISlider();
    p_pitchSlider->SetMinMaxValues(0.0f, 5.0f);
    p_pitchSlider->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_loopingCheckbox = GameObjectFactory::CreateUICheckBox();
    p_loopingCheckbox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_playOnStartCheckbox = GameObjectFactory::CreateUICheckBox();
    p_playOnStartCheckbox->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_playStopButton = GameObjectFactory::CreateUIButton("Play");
    p_playStopButton->AddClickedCallback([this]()
    {
        OnPlayClicked();
    });

    AddWidget("Audio clip",    p_audioClipFileInput);
    AddWidget("Range",         p_rangeInput->GetGameObject());
    AddWidget("Volume",        p_volumeSlider->GetGameObject());
    AddWidget("Pitch",         p_pitchSlider->GetGameObject());
    AddWidget("Looping",       p_loopingCheckbox->GetGameObject());
    AddWidget("Play on Start", p_playOnStartCheckbox->GetGameObject());
    AddWidget(p_playStopButton->GetGameObject());

    SetLabelsWidth(75);
}

void CIWAudioSource::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    AudioSource *as = GetAudioSource();
    AudioClip *ac = as->GetAudioClip();
    p_audioClipFileInput->SetPath( ac ? ac->GetSoundFilepath() : Path::Empty );

    p_rangeInput->SetValue( as->GetRange() );
    p_volumeSlider->SetValue( as->GetVolume() );
    p_pitchSlider->SetValue( as->GetPitch() );
    p_loopingCheckbox->SetChecked( as->GetLooping() );
    p_playOnStartCheckbox->SetChecked( as->GetPlayOnStart() );

    p_playStopButton->SetBlocked(!ac || !ac->IsLoaded());
    p_playStopButton->GetText()->SetContent(as->IsPlaying() ? "Stop" : "Play");
}

void CIWAudioSource::OnPlayClicked()
{
    if (!GetAudioSource()->IsPlaying())
    {
        GetAudioSource()->Play();
    }
    else
    {
        GetAudioSource()->Stop();
    }
}

void CIWAudioSource::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    if (object == p_audioClipFileInput)
    {
        RH<AudioClip> ac = Resources::Load<AudioClip>( p_audioClipFileInput->GetPath() );
        GetAudioSource()->SetAudioClip( ac.Get() );
    }

    GetAudioSource()->SetRange( p_rangeInput->GetValue() );
    GetAudioSource()->SetVolume( p_volumeSlider->GetValue() );
    GetAudioSource()->SetPitch ( p_pitchSlider->GetValue() );
    GetAudioSource()->SetLooping( p_loopingCheckbox->IsChecked() );
    GetAudioSource()->SetPlayOnStart( p_playOnStartCheckbox->IsChecked() );
}

AudioSource *CIWAudioSource::GetAudioSource() const
{
    return SCAST<AudioSource*>( ComponentInspectorWidget::GetComponent() );
}
