#ifndef RIWANIMATION_H
#define RIWANIMATION_H

#include <vector>

#include "Bang/Animation.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/RIWResource.h"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
class UIComboBox;
class UIInputNumber;
class UIInputText;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class RIWAnimation : public RIWResource<Animation>
{
    GAMEOBJECT_EDITOR(RIWAnimation);

public:
    // InspectorWidget
    void Init() override;

private:
    UIInputNumber *p_speedInput = nullptr;
    UIComboBox *p_wrapModeInput = nullptr;

    UIInputText *p_durationInSeconds = nullptr;

    RIWAnimation();
    virtual ~RIWAnimation() override;

    Animation *GetAnimation() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedRIWResource(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};
}

#endif  // RIWANIMATION_H
