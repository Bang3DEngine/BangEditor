#ifndef CIWCAPSULECOLLIDER_H
#define CIWCAPSULECOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class CapsuleCollider;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

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
    UIComboBox *p_axisInput = nullptr;

    CIWCapsuleCollider();
    virtual ~CIWCapsuleCollider();

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    CapsuleCollider *GetCapsuleCollider() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWCAPSULECOLLIDER_H

