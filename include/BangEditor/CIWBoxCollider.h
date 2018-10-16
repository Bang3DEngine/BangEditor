#ifndef CIWBOXCOLLIDER_H
#define CIWBOXCOLLIDER_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/CIWCollider.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class BoxCollider;
FORWARD   class IEventsValueChanged;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;

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
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    BoxCollider *GetBoxCollider() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWBOXCOLLIDER_H

