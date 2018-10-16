#ifndef CIWRIGIDBODY_H
#define CIWRIGIDBODY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class RigidBody;
FORWARD   class UICheckBox;
FORWARD   class UIComboBox;
FORWARD   class UIInputNumber;
FORWARD   class UIInputVector;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class CIWRigidBody : public ComponentInspectorWidget
{
    GAMEOBJECT_EDITOR(CIWRigidBody);

public:
    // InspectorWidget
    virtual void InitInnerWidgets() override;

    // ComponentInspectorWidget
    virtual void UpdateFromReference() override;

private:
    UIInputNumber *p_massInput = nullptr;
    UIInputNumber *p_dragInput = nullptr;
    UIInputNumber *p_angularDragInput = nullptr;
    UICheckBox *p_useGravityInput = nullptr;
    UICheckBox *p_isKinematicInput = nullptr;
    UIComboBox *p_constraintsInput = nullptr;

    CIWRigidBody();
    virtual ~CIWRigidBody() override;

    // ComponentInspectorWidget
    virtual void OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object) override;

    RigidBody *GetRigidBody() const;

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // CIWRIGIDBODY_H

