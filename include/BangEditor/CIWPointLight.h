#ifndef CIWPOINTLIGHT_H
#define CIWPOINTLIGHT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWLight.h"

namespace Bang
{
class IEventsValueChanged;
class PointLight;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class CIWPointLight : public CIWLight
{
    GAMEOBJECT_EDITOR(CIWPointLight);

public:
    CIWPointLight() = default;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_rangeInput = nullptr;

    PointLight *GetPointLight() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    virtual ~CIWPointLight() override = default;
};
}

#endif  // CIWPOINTLIGHT_H
