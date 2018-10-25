#ifndef ASMTRANSITIONCONDITIONINPUT_H
#define ASMTRANSITIONCONDITIONINPUT_H

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
class AnimatorStateMachineNode;
class AnimatorStateMachineTransition;
class AnimatorStateMachineLayer;
class IEventsValueChanged;
class UIComboBox;
class UIInputNumber;
}

using namespace Bang;
namespace BangEditor
{
class ASMTransitionConditionInput : public GameObject,
                                    public EventEmitter<IEventsValueChanged>,
                                    public EventListener<IEventsValueChanged>
{
public:
    ASMTransitionConditionInput();
    virtual ~ASMTransitionConditionInput() override;

    // GameObject
    void BeforeRender() override;

    void SetVariableType(AnimatorStateMachineVariable::Type type);
    void SetStateMachineTransition(AnimatorStateMachineTransition *transition);

    AnimatorStateMachine *GetStateMachine() const;
    AnimatorStateMachineLayer *GetStateMachineLayer() const;
    AnimatorStateMachineNode *GetStateMachineNode() const;
    AnimatorStateMachineTransition *GetStateMachineTransition() const;

private:
    String m_selectedVarName = "";
    bool m_updatingFromVariable = false;
    AnimatorStateMachineTransition *p_stateMachineTransition = nullptr;
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

#endif  // ASMTRANSITIONCONDITIONINPUT_H
