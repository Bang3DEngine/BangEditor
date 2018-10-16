#ifndef CIWROPE_H
#define CIWROPE_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWRenderer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class Rope;
FORWARD   class UIInputNumber;
FORWARD   class UICheckBox;
FORWARD   class UISlider;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWRope : public CIWRenderer
{
    GAMEOBJECT_EDITOR(CIWRope);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

private:
    UIInputNumber *p_numPointsInput      = nullptr;
    UISlider      *p_bouncinessInput     = nullptr;
    UISlider      *p_dampingInput        = nullptr;
    UIInputNumber *p_springsForceInput   = nullptr;
    UIInputNumber *p_ropeLengthInput     = nullptr;
    UICheckBox    *p_seeDebugPointsInput = nullptr;

    CIWRope();
    virtual ~CIWRope() override;

    Rope *GetRope() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWROPE_H

