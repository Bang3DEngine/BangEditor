#ifndef CIWLIGHT_H
#define CIWLIGHT_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class Light;
FORWARD   class UIComboBox;
FORWARD   class UIInputNumber;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;

class CIWLight : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWLight);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWLight() = default;
    virtual ~CIWLight() override = default;

    Light *GetLight() const;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputNumber *p_intensityInput = nullptr;
    UIInputColor *p_colorInput = nullptr;
    UIComboBox *p_shadowTypeInput = nullptr;
    UISlider *p_shadowBiasInput = nullptr;
    UISlider *p_shadowMapSizeInput = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWLIGHT_H

