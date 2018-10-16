#ifndef CIWAUDIOSOURCE_H
#define CIWAUDIOSOURCE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class AudioSource;
FORWARD   class IEventsValueChanged;
FORWARD   class UICheckBox;
FORWARD   class UIButton;
FORWARD   class UIInputNumber;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputFileWithPreview;

class CIWAudioSource : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAudioSource);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_rangeInput                   = nullptr;
    UISlider      *p_pitchSlider                  = nullptr;
    UISlider      *p_volumeSlider                 = nullptr;
    UIButton      *p_playStopButton               = nullptr;
    UICheckBox    *p_loopingCheckbox              = nullptr;
    UICheckBox    *p_playOnStartCheckbox          = nullptr;
    UIInputFileWithPreview *p_audioClipFileInput  = nullptr;

    void OnPlayClicked();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    AudioSource *GetAudioSource() const;

    CIWAudioSource() = default;
    virtual ~CIWAudioSource() override = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWAUDIOSOURCE_H

