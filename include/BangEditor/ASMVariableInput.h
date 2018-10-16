#ifndef ASMVARIABLEINPUT_H
#define ASMVARIABLEINPUT_H

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

namespace Bang {
class IEventsValueChanged;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UICheckBox;
FORWARD class UIComboBox;
FORWARD class UIInputNumber;
FORWARD class UIInputText;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ASMVariableInput : public GameObject,
                         public EventEmitter<IEventsValueChanged>,
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

