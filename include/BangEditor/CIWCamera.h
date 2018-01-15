#ifndef CIWCAMERA_H
#define CIWCAMERA_H

#include "Bang/Bang.h"

#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UISlider;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;

class CIWCamera : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWCamera);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    CIWCamera() = default;
    virtual ~CIWCamera() = default;

    Camera *GetCamera() const;

    // IValueChangedListener
    virtual void OnValueChanged(Object *object) override;

private:
    UIInputColor *p_clearColorInput = nullptr;
    UIInputNumber *p_zNearInput = nullptr;
    UIInputNumber *p_zFarInput = nullptr;
    UIInputNumber *p_orthoHeightInput = nullptr;
    UISlider      *p_fovInput = nullptr;
    UIComboBox *p_projectionModeInput = nullptr;

};

NAMESPACE_BANG_EDITOR_END

#endif // CIWCAMERA_H

