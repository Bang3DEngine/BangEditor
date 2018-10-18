#ifndef CIWAUDIOSOURCE_H
#define CIWAUDIOSOURCE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class AudioSource;
class IEventsValueChanged;
class UICheckBox;
class UIButton;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputFileWithPreview;

class CIWAudioSource : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWAudioSource);

public:
    CIWAudioSource() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_rangeInput = nullptr;
    UISlider *p_pitchSlider = nullptr;
    UISlider *p_volumeSlider = nullptr;
    UIButton *p_playStopButton = nullptr;
    UICheckBox *p_loopingCheckbox = nullptr;
    UICheckBox *p_playOnStartCheckbox = nullptr;
    UIInputFileWithPreview *p_audioClipFileInput = nullptr;

    void OnPlayClicked();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    AudioSource *GetAudioSource() const;

    virtual ~CIWAudioSource() override = default;
};
}

#endif  // CIWAUDIOSOURCE_H
