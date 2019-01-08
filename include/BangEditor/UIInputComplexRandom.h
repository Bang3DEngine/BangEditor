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

namespace Bang
{
class IEventsValueChanged;
class UIInputNumber;
}

using namespace Bang;
namespace BangEditor
{
class UIInputComplexRandom : public GameObject,
                             public EventListener<IEventsValueChanged>,
                             public EventEmitter<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UIInputComplexRandom);

public:
    UIInputComplexRandom();
    virtual ~UIInputComplexRandom() override;

    void Set(const ComplexRandom &complexRandom);

    void SetRangeMinValue(float minValue);
    void SetRangeMaxValue(float maxValue);
    void SetRangeMinMaxValues(float minValue, float maxValue);

    bool HasFocus() const;
    const ComplexRandom &GetComplexRandom() const;

private:
    ComplexRandom m_complexRandom;

    UIInputNumber *p_minRangeInputNumber = nullptr;
    UIInputNumber *p_maxRangeInputNumber = nullptr;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;
};
}

#endif  // UIINPUTCOMPLEXRANDOM_H
