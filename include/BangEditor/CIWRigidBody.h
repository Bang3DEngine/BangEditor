#ifndef CIWRIGIDBODY_H
#define CIWRIGIDBODY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class RigidBody;
class UICheckBox;
class UIComboBox;
class UIInputNumber;
class UIInputVector;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
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
    virtual void OnValueChangedCIW(
        EventEmitter<IEventsValueChanged> *object) override;

    RigidBody *GetRigidBody() const;

    friend class ComponentInspectorWidgetFactory;
};
}

#endif  // CIWRIGIDBODY_H
