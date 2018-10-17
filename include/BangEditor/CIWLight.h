#ifndef CIWLIGHT_H
#define CIWLIGHT_H

#include "Bang/BangDefines.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class Light;
class UIComboBox;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputColor;

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
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputNumber *p_intensityInput = nullptr;
    UIInputColor *p_colorInput = nullptr;
    UIComboBox *p_shadowTypeInput = nullptr;
    UISlider *p_shadowBiasInput = nullptr;
    UISlider *p_shadowMapSizeInput = nullptr;
};
}

#endif  // CIWLIGHT_H
