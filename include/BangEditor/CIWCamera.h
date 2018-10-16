#ifndef CIWCAMERA_H
#define CIWCAMERA_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang {
class Camera;
class IEventsValueChanged;
class UICheckBox;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class UISlider;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputColor;
FORWARD class UIInputFileWithPreview;

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

    void LimitValues();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputNumber          *p_zNearInput          = nullptr;
    UIInputNumber          *p_zFarInput           = nullptr;
    UIInputNumber          *p_orthoHeightInput    = nullptr;
    UISlider               *p_fovInput            = nullptr;
    UIComboBox             *p_projectionModeInput = nullptr;
    UIComboBox             *p_clearModeInput      = nullptr;
    UICheckBox             *p_isActiveCamera       = nullptr;
    UIInputColor           *p_clearColorInput     = nullptr;
    UIInputFileWithPreview *p_textureCubeMapInput = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWCAMERA_H

