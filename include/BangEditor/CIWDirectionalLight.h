#ifndef CIWDIRECTIONALLIGHT_H
#define CIWDIRECTIONALLIGHT_H

#include "Bang/Bang.h"

#include "BangEditor/CIWLight.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UIInputNumber;
FORWARD class DirectionalLight;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class CIWDirectionalLight : public CIWLight
{
    GAMEOBJECT_EDITOR(CIWDirectionalLight);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_shadowDistanceInput = nullptr;

    DirectionalLight *GetDirectionalLight() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    CIWDirectionalLight() = default;
    virtual ~CIWDirectionalLight() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWDIRECTIONALLIGHT_H

