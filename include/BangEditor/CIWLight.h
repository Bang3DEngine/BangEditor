#ifndef CIWLIGHT_H
#define CIWLIGHT_H

#include "Bang/IValueChangedListener.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Light;
FORWARD class UIInputNumber;
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
    virtual ~CIWLight() = default;

    Light *GetLight() const;

    // IValueChangedListener
    virtual void OnValueChanged(Object *object) override;

private:
    UIInputNumber *p_intensityInput = nullptr;
    UIInputColor *p_colorInput = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWLIGHT_H

