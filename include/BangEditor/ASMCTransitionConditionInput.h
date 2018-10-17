#ifndef ASMCTRANSITIONCONDITIONINPUT_H
#define ASMCTRANSITIONCONDITIONINPUT_H

#include <vector>

#include "Bang/AnimatorStateMachineVariable.h"
#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/MetaNode.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class AnimatorStateMachine;
class IEventsValueChanged;
class UIComboBox;
class UIInputNumber;
}

using namespace Bang;
namespace BangEditor
{
class ASMCTransitionConditionInput : public GameObject,
                                     public EventEmitter<IEventsValueChanged>,
                                     public EventListener<IEventsValueChanged>
{
public:
    ASMCTransitionConditionInput();
    virtual ~ASMCTransitionConditionInput() override;

    // GameObject
    void BeforeRender() override;

    void SetVariableType(AnimatorStateMachineVariable::Type type);
    void SetAnimatorStateMachine(AnimatorStateMachine *animatorSM);

private:
    String m_selectedVarName = "";
    bool m_updatingFromVariable = false;
    AnimatorStateMachine *p_animatorSM = nullptr;
    UIComboBox *p_varNameInput = nullptr;
    UIComboBox *p_comparatorInput = nullptr;
    UIInputNumber *p_floatInput = nullptr;
    AnimatorStateMachineVariable::Type m_varType =
        Undef<AnimatorStateMachineVariable::Type>();

    void UpdateFromVariable();

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;
};
}

#endif  // ASMCTRANSITIONCONDITIONINPUT_H
