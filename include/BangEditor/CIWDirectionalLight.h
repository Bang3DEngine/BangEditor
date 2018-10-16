#ifndef CIWDIRECTIONALLIGHT_H
#define CIWDIRECTIONALLIGHT_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWLight.h"

namespace Bang {
class IEventsValueChanged;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class DirectionalLight;
FORWARD class UIInputNumber;
FORWARD class UISlider;

FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

class CIWDirectionalLight : public CIWLight
{
    GAMEOBJECT_EDITOR(CIWDirectionalLight);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_shadowDistanceInput = nullptr;

    DirectionalLight *GetDirectionalLight() const;

    // ComponentInspectorWidget
    void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    CIWDirectionalLight() = default;
    virtual ~CIWDirectionalLight() = default;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWDIRECTIONALLIGHT_H

