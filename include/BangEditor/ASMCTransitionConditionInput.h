#ifndef ASMCTRANSITIONCONDITIONINPUT_H
#define ASMCTRANSITIONCONDITIONINPUT_H

#include "Bang/Bang.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/AnimatorStateMachineVariable.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class AnimatorStateMachine;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ASMCTransitionConditionInput : public GameObject,
                                     public EventEmitter<IEventsValueChanged>,
                                     public EventListener<IEventsValueChanged>
{
public:
	ASMCTransitionConditionInput();
	virtual ~ASMCTransitionConditionInput();

    // GameObject
    void BeforeRender() override;

    void SetVariableType(AnimatorStateMachineVariable::Type type);
    void SetAnimatorStateMachine(AnimatorStateMachine *animatorSM);

private:
    String m_selectedVarName = "";
    bool m_updatingFromVariable = false;
    AnimatorStateMachine *p_animatorSM = nullptr;
    UIComboBox    *p_varNameInput      = nullptr;
    UIComboBox    *p_comparatorInput   = nullptr;
    UIInputNumber *p_floatInput        = nullptr;
    AnimatorStateMachineVariable::Type m_varType =
            Undef<AnimatorStateMachineVariable::Type>();

    void UpdateFromVariable();

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;
};

NAMESPACE_BANG_EDITOR_END

#endif // ASMCTRANSITIONCONDITIONINPUT_H

