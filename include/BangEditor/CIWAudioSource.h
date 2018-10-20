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
    UIButton *p_playStopButton = nullptr;

    void OnPlayClicked();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    AudioSource *GetAudioSource() const;

    virtual ~CIWAudioSource() override = default;
};
}

#endif  // CIWAUDIOSOURCE_H
