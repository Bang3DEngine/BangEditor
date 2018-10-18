#ifndef CIWROPE_H
#define CIWROPE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

namespace Bang
{
class IEventsValueChanged;
class Rope;
class UIInputNumber;
class UICheckBox;
class UISlider;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class CIWRope : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWRope);

public:
    CIWRope();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

private:
    UISlider *p_bouncinessInput = nullptr;
    UISlider *p_dampingInput = nullptr;
    UIInputNumber *p_springsForceInput = nullptr;
    UIInputNumber *p_ropeLengthInput = nullptr;
    UICheckBox *p_seeDebugPointsInput = nullptr;

    virtual ~CIWRope() override;

    Rope *GetRope() const;
};
}

#endif  // CIWROPE_H
