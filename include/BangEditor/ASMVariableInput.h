#ifndef ASMVARIABLEINPUT_H
#define ASMVARIABLEINPUT_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/AnimatorStateMachineVariable.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIInputText;
FORWARD class UIInputNumber;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ASMVariableInput : public GameObject,
                         public EventListener<IEventsValueChanged>
{
    GAMEOBJECT_EDITOR(ASMVariableInput);

public:
	ASMVariableInput();
	virtual ~ASMVariableInput();

private:
    UIInputText   *p_varNameInput = nullptr;
    UIComboBox    *p_varTypeInput = nullptr;
    UIInputNumber *p_floatInput   = nullptr;
    UICheckBox    *p_boolInput    = nullptr;

    AnimatorStateMachineVariable::Type m_varType =
                    Undef<AnimatorStateMachineVariable::Type>();

    void SetVarType(AnimatorStateMachineVariable::Type type);

    AnimatorStateMachineVariable::Type GetVarType() const;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // ASMVARIABLEINPUT_H

