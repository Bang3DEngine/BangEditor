#include "BangEditor/CIWAudioSource.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AudioClip.h"
#include "Bang/AudioSource.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEvents.h"
#include "Bang/Path.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UISlider.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/UIInputFileWithPreview.h"

namespace Bang
{
class IEventsValueChanged;
}

using namespace Bang;
using namespace BangEditor;

void CIWAudioSource::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWAudioSource");
    SetTitle("AudioSource");

    p_playStopButton = GameObjectFactory::CreateUIButton("Play");
    p_playStopButton->AddClickedCallback([this]() { OnPlayClicked(); });

    AddWidget(p_playStopButton->GetGameObject(), -1);

    SetLabelsWidth(75);
}

void CIWAudioSource::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    AudioSource *as = GetAudioSource();
    AudioClip *ac = as->GetAudioClip();

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

void CIWAudioSource::OnValueChangedCIW(
    EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);
}

AudioSource *CIWAudioSource::GetAudioSource() const
{
    return SCAST<AudioSource *>(ComponentInspectorWidget::GetComponent());
}
