#ifndef CIWPOSTPROCESSEFFECTSSAO_H
#define CIWPOSTPROCESSEFFECTSSAO_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class PostProcessEffectSSAO;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWPostProcessEffectSSAO : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWPostProcessEffectSSAO);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UISlider *p_intensityInput = nullptr;
    UIInputNumber *p_radiusInput = nullptr;
    UISlider *p_blurRadiusInput = nullptr;
    UISlider *p_numRandAxesInput = nullptr;
    UISlider *p_numRandSamplesInput = nullptr;
    UICheckBox *p_bilateralBlurInput = nullptr;
    UICheckBox *p_separableInput = nullptr;

    CIWPostProcessEffectSSAO();
    virtual ~CIWPostProcessEffectSSAO();

    PostProcessEffectSSAO* GetPostProcessEffectSSAO() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPOSTPROCESSEFFECTSSAO_H

