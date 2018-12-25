#ifndef AIWAUDIOCLIP_H
#define AIWAUDIOCLIP_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AudioClip.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class ALAudioSource;
class IEventsDestroy;
class IEventsValueChanged;
class Texture2D;
class UIButton;
class UIInputNumber;
class UIInputText;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class AIWAudioClip : public AIWAsset<AudioClip>
{
    GAMEOBJECT_EDITOR(AIWAudioClip);

public:
    AIWAudioClip();

    // InspectorWidget
    void Init() override;

private:
    UIButton *p_playStopButton = nullptr;
    ALAudioSource *p_alAudioSourceBeingPlayed = nullptr;

    UIInputText *p_duration = nullptr;
    UIInputText *p_frequency = nullptr;
    UIInputNumber *p_bitDepth = nullptr;
    UIInputNumber *p_bufferSize = nullptr;
    UIInputNumber *p_numChannels = nullptr;

    virtual ~AIWAudioClip() override;

    void Play();
    void Stop();
    AudioClip *GetAudioClip() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;
    Color GetIconTint() const override;

    // AIWAsset
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // AIWAUDIOCLIP_H
