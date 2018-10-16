#ifndef CIWPOINTLIGHT_H
#define CIWPOINTLIGHT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWLight.h"

namespace Bang {
class IEventsValueChanged;
class UIInputNumber;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class PointLight;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWPointLight : public CIWLight
{
    GAMEOBJECT_EDITOR(CIWPointLight);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_rangeInput = nullptr;

    PointLight *GetPointLight() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    CIWPointLight() = default;
    virtual ~CIWPointLight() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWPOINTLIGHT_H

