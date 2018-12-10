#ifndef CIWREFLECTIONPROBE_H
#define CIWREFLECTIONPROBE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "Bang/UIInputNumber.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"
#include "BangEditor/UIInputVector.h"

namespace Bang
{
class IEventsValueChanged;
class ReflectionProbe;
class UIComboBox;
class UICheckBox;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputColor;
class UIInputFile;
class UIInputVector;
class UITextureCubeMapPreviewer;

class CIWReflectionProbe : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWReflectionProbe);

public:
    CIWReflectionProbe();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    void LimitValues();

private:
    UIComboBox *p_reflectionProbSizeInput = nullptr;
    UIInputVector *p_sizeInput = nullptr;
    UICheckBox *p_isBoxedCheckBox = nullptr;
    UIInputNumber *p_restTimeInput = nullptr;
    UICheckBox *p_filterForIBLCheckBox = nullptr;

    UIInputNumber *p_zNearInput = nullptr;
    UIInputNumber *p_zFarInput = nullptr;
    UIComboBox *p_clearModeInput = nullptr;
    UIInputColor *p_clearColorInput = nullptr;
    UIInputFile *p_textureCubeMapInput = nullptr;

    UITextureCubeMapPreviewer *p_previewCMRenderer = nullptr;

    virtual ~CIWReflectionProbe() override;

    ReflectionProbe *GetReflectionProbe() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;
};
}

#endif  // CIWREFLECTIONPROBE_H
