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

namespace Bang {
class IEventsValueChanged;
class Texture2D;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class UIInputText;
FORWARD class UISlider;

FORWARD NAMESPACE_BANG_END


USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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
    virtual ~RIWAnimation();

    Animation *GetAnimation() const;

    // RIWResource
    void UpdateInputsFromResource() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedRIWResource(
                        EventEmitter<IEventsValueChanged> *object) override;

    friend class ResourceInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // RIWANIMATION_H

