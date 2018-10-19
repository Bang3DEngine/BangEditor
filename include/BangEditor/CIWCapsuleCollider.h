#ifndef CIWCAPSULECOLLIDER_H
#define CIWCAPSULECOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace Bang
{
class CapsuleCollider;
class IEventsValueChanged;
class UIComboBox;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class CIWCapsuleCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWCapsuleCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_radiusInput = nullptr;
    UIInputNumber *p_heightInput = nullptr;

    CIWCapsuleCollider();
    virtual ~CIWCapsuleCollider() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    CapsuleCollider *GetCapsuleCollider() const;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWCAPSULECOLLIDER_H
