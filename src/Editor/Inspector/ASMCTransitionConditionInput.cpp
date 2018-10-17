#include "BangEditor/ASMCTransitionConditionInput.h"

#include <sys/types.h>
#include <ostream>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineConnectionTransitionCondition.h"
#include "Bang/Array.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/MetaNode.h"
#include "Bang/MetaNode.tcc"
#include "Bang/Serializable.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"

using namespace Bang;
using namespace BangEditor;

ASMCTransitionConditionInput::ASMCTransitionConditionInput()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(20);
    le->SetFlexibleWidth(1.0f);

    p_varNameInput = GameObjectFactory::CreateUIComboBox();
    p_varNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_comparatorInput = GameObjectFactory::CreateUIComboBox();
    p_comparatorInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_floatInput = GameObjectFactory::CreateUIInputNumber();
    p_floatInput->SetValue(0.0f);
    p_floatInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_varNameInput->GetGameObject()->SetParent(this);
    p_comparatorInput->GetGameObject()->SetParent(this);
    p_floatInput->GetGameObject()->SetParent(this);
}

ASMCTransitionConditionInput::~ASMCTransitionConditionInput()
{
}

void ASMCTransitionConditionInput::BeforeRender()
{
    GameObject::BeforeRender();

    if(p_animatorSM)
    {
        UpdateFromVariable();
        p_varNameInput->SetSelectionByLabel(m_selectedVarName);
    }
}

void ASMCTransitionConditionInput::SetVariableType(
    AnimatorStateMachineVariable::Type type)
{
    if(type != m_varType)
    {
        m_varType = type;

        p_comparatorInput->ClearItems();
        switch(m_varType)
        {
            case AnimatorStateMachineVariable::Type::BOOL:
                p_floatInput->GetGameObject()->SetEnabled(false);
                p_comparatorInput->AddItem(
                    "True",
                    SCAST<uint>(ASMCTransitionCondition::Comparator::IS_TRUE));
                p_comparatorInput->AddItem(
                    "False",
                    SCAST<uint>(ASMCTransitionCondition::Comparator::IS_FALSE));

                break;

            case AnimatorStateMachineVariable::Type::FLOAT:
                p_floatInput->GetGameObject()->SetEnabled(true);
                p_comparatorInput->AddItem(
                    "Greater",
                    SCAST<uint>(ASMCTransitionCondition::Comparator::GREATER));
                p_comparatorInput->AddItem(
                    "Less",
                    SCAST<uint>(ASMCTransitionCondition::Comparator::LESS));
                break;
        }
    }
}

void ASMCTransitionConditionInput::SetAnimatorStateMachine(
    AnimatorStateMachine *animatorSM)
{
    if(animatorSM != p_animatorSM)
    {
        p_animatorSM = animatorSM;
        UpdateFromVariable();
    }
}

void ASMCTransitionConditionInput::UpdateFromVariable()
{
    if(p_animatorSM && !m_updatingFromVariable)
    {
        m_updatingFromVariable = true;

        bool updateVariablesComboBox = false;
        updateVariablesComboBox |= (p_animatorSM->GetVariables().Size() !=
                                    p_varNameInput->GetNumItems());
        if(!updateVariablesComboBox)
        {
            const auto &refVars = p_animatorSM->GetVariables();
            const auto &comboLabels = p_varNameInput->GetLabels();
            for(uint i = 0; i < refVars.Size(); ++i)
            {
                if(refVars[i]->GetName() != comboLabels[i])
                {
                    updateVariablesComboBox = true;
                    break;
                }
            }
        }

        if(updateVariablesComboBox)
        {
            p_varNameInput->ClearItems();
            for(AnimatorStateMachineVariable *var :
                p_animatorSM->GetVariables())
            {
                p_varNameInput->AddItem(var->GetName(), 0);
            }
        }

        if(AnimatorStateMachineVariable *var =
               p_animatorSM->GetVariable(m_selectedVarName))
        {
            SetVariableType(var->GetType());
        }

        m_updatingFromVariable = false;
    }
}

void ASMCTransitionConditionInput::OnValueChanged(
    EventEmitter<IEventsValueChanged> *ee)
{
    if(ee == p_varNameInput)
    {
        m_selectedVarName = p_varNameInput->GetSelectedLabel();
    }
    UpdateFromVariable();

    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void ASMCTransitionConditionInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if(metaNode.Contains("VariableName"))
    {
        m_selectedVarName = metaNode.Get<String>("VariableName");
        p_varNameInput->SetSelectionByLabel(m_selectedVarName);
    }

    if(metaNode.Contains("Comparator"))
    {
        p_comparatorInput->SetSelectionByValue(
            metaNode.Get<uint>("Comparator"));
    }

    if(metaNode.Contains("CompareValueFloat"))
    {
        p_floatInput->SetValue(metaNode.Get<float>("CompareValueFloat"));
    }

    UpdateFromVariable();
}

void ASMCTransitionConditionInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("VariableName", p_varNameInput->GetSelectedLabel());
    metaNode->Set("Comparator", p_comparatorInput->GetSelectedValue());
    metaNode->Set("CompareValueFloat", p_floatInput->GetValue());
}
