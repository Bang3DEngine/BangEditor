#ifndef CIWBOXCOLLIDER_H
#define CIWBOXCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace Bang
{
class BoxCollider;
class IEventsValueChanged;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputVector;

class CIWBoxCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWBoxCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputVector *p_extentsInput = nullptr;

    CIWBoxCollider();
    virtual ~CIWBoxCollider() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    BoxCollider *GetBoxCollider() const;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWBOXCOLLIDER_H
