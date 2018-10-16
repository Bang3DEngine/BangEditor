#ifndef UIINPUTCOMPLEXRANDOM_H
#define UIINPUTCOMPLEXRANDOM_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/ComplexRandom.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class IEventsValueChanged;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIInputNumber;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UIInputComplexRandom : public GameObject,
                             public EventListener<IEventsValueChanged>,
                             public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(UIInputComplexRandom);

public:
	UIInputComplexRandom();
	virtual ~UIInputComplexRandom();

    void Set(const ComplexRandom &complexRandom);

    void SetRangeMinValue(float minValue);
    void SetRangeMaxValue(float maxValue);
    void SetRangeMinMaxValues(float minValue, float maxValue);

    bool HasFocus() const;
    const ComplexRandom& GetComplexRandom() const;

private:
    ComplexRandom m_complexRandom;

    UIInputNumber *p_minRangeInputNumber = nullptr;
    UIInputNumber *p_maxRangeInputNumber = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UIINPUTCOMPLEXRANDOM_H

