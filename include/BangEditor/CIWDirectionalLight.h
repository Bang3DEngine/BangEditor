#ifndef CIWDIRECTIONALLIGHT_H
#define CIWDIRECTIONALLIGHT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWLight.h"

namespace Bang
{
class DirectionalLight;
class IEventsValueChanged;
class UIInputNumber;
class UISlider;
template <class>
class EventEmitter;
}

namespace BangEditor
{
class CIWDirectionalLight : public CIWLight
{
    GAMEOBJECT_EDITOR(CIWDirectionalLight);

public:
    CIWDirectionalLight() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_shadowDistanceInput = nullptr;

    DirectionalLight *GetDirectionalLight() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    virtual ~CIWDirectionalLight() override = default;
};
}

#endif  // CIWDIRECTIONALLIGHT_H
