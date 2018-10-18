#ifndef CIWPOSTPROCESSEFFECTSSAO_H
#define CIWPOSTPROCESSEFFECTSSAO_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class PostProcessEffectSSAO;
class UIComboBox;
class UICheckBox;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class CIWPostProcessEffectSSAO : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWPostProcessEffectSSAO);

public:
    CIWPostProcessEffectSSAO();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

private:
    UISlider *p_intensityInput = nullptr;
    UIInputNumber *p_radiusInput = nullptr;
    UISlider *p_blurRadiusInput = nullptr;
    UISlider *p_numRandAxesInput = nullptr;
    UISlider *p_numRandSamplesInput = nullptr;
    UICheckBox *p_bilateralBlurInput = nullptr;
    UICheckBox *p_separableInput = nullptr;

    virtual ~CIWPostProcessEffectSSAO() override;

    PostProcessEffectSSAO *GetPostProcessEffectSSAO() const;
};
}

#endif  // CIWPOSTPROCESSEFFECTSSAO_H
