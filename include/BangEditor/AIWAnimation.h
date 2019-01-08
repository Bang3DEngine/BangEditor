#ifndef AIWANIMATION_H
#define AIWANIMATION_H

#include <vector>

#include "Bang/Animation.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/IEvents.h"
#include "Bang/String.h"
#include "BangEditor/AIWAsset.h"
#include "BangEditor/BangEditor.h"

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
class AIWAnimation : public AIWAsset<Animation>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(AIWAnimation);

public:
    // InspectorWidget
    void Init() override;

private:
    UIInputNumber *p_speedInput = nullptr;
    UIComboBox *p_wrapModeInput = nullptr;

    UIInputText *p_durationInSeconds = nullptr;

    AIWAnimation();
    virtual ~AIWAnimation() override;

    Animation *GetAnimation() const;

    // AIWAsset
    void UpdateInputsFromAsset() override;
    Texture2D *GetIconTexture() const override;
    void OnValueChangedAIWAsset(
        EventEmitter<IEventsValueChanged> *object) override;

    friend class AssetInspectorWidgetFactory;
};
}

#endif  // AIWANIMATION_H
