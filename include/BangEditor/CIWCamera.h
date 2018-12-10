#ifndef CIWCAMERA_H
#define CIWCAMERA_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class Camera;
class IEventsValueChanged;
class UICheckBox;
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
class UIInputFile;

class CIWCamera : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWCamera);

public:
    CIWCamera() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

protected:
    virtual ~CIWCamera() override = default;

    Camera *GetCamera() const;

    void LimitValues();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputNumber *p_zNearInput = nullptr;
    UIInputNumber *p_zFarInput = nullptr;
    UIInputNumber *p_orthoHeightInput = nullptr;
    UISlider *p_fovInput = nullptr;
    UIComboBox *p_projectionModeInput = nullptr;
    UIComboBox *p_clearModeInput = nullptr;
    UICheckBox *p_isActiveCamera = nullptr;
    UIInputColor *p_clearColorInput = nullptr;
    UIInputFile *p_textureCubeMapInput = nullptr;
};
}

#endif  // CIWCAMERA_H
