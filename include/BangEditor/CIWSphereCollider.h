#ifndef CIWSPHERECOLLIDER_H
#define CIWSPHERECOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

namespace Bang
{
class IEventsValueChanged;
class SphereCollider;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class CIWSphereCollider : public CIWCollider
{
    GAMEOBJECT_EDITOR(CIWSphereCollider);

    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_radiusInput = nullptr;

    CIWSphereCollider();
    virtual ~CIWSphereCollider() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    SphereCollider *GetSphereCollider() const;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWSPHERECOLLIDER_H
