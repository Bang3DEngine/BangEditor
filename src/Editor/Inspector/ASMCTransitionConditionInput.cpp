#include "BangEditor/ASMCTransitionConditionInput.h"

#include "Bang/MetaNode.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineConnectionTransitionCondition.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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
    p_comparatorInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

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

void ASMCTransitionConditionInput::Update()
{
    GameObject::Update();

    if (p_animatorSM)
    {
        UpdateFromVariable();
        p_varNameInput->SetSelectionByLabel(m_selectedVarName);
    }
}

void ASMCTransitionConditionInput::SetVariableType(
                                    AnimatorStateMachineVariable::Type type)
{
    if (type != m_varType)
    {
        m_varType = type;

        p_comparatorInput->ClearItems();
        switch (m_varType)
        {
            case AnimatorStateMachineVariable::Type::BOOL:
                p_floatInput->GetGameObject()->SetEnabled(false);
                p_comparatorInput->AddItem("True",
                        SCAST<uint>(ASMCTransitionCondition::Comparator::IS_TRUE));
                p_comparatorInput->AddItem("False",
                        SCAST<uint>(ASMCTransitionCondition::Comparator::IS_FALSE));

            break;

            case AnimatorStateMachineVariable::Type::FLOAT:
                p_floatInput->GetGameObject()->SetEnabled(true);
                p_comparatorInput->AddItem("Greater",
                        SCAST<uint>(ASMCTransitionCondition::Comparator::GREATER));
                p_comparatorInput->AddItem("Less",
                        SCAST<uint>(ASMCTransitionCondition::Comparator::LESS));
            break;
        }
    }
}

void ASMCTransitionConditionInput::SetAnimatorStateMachine(
                                        AnimatorStateMachine *animatorSM)
{
    if (animatorSM != p_animatorSM)
    {
        p_animatorSM = animatorSM;
        UpdateFromVariable();
    }
}

void ASMCTransitionConditionInput::UpdateFromVariable()
{
    if (p_animatorSM && !m_updatingFromVariable)
    {
        m_updatingFromVariable = true;
        if (p_animatorSM->GetVariables().Size() != p_varNameInput->GetNumItems())
        {
            p_varNameInput->ClearItems();
            for (AnimatorStateMachineVariable *var : p_animatorSM->GetVariables())
            {
                p_varNameInput->AddItem(var->GetName(), 0);
            }
        }

        if (AnimatorStateMachineVariable *var =
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
    if (ee == p_varNameInput)
    {
        m_selectedVarName = p_varNameInput->GetSelectedLabel();
        UpdateFromVariable();
    }

    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                 &IEventsValueChanged::OnValueChanged, this);
}

void ASMCTransitionConditionInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if (metaNode.Contains("VariableName"))
    {
        m_selectedVarName = metaNode.Get<String>("VariableName");
    }

    if (metaNode.Contains("Comparator"))
    {
        p_comparatorInput->SetSelectionByValue( metaNode.Get<uint>("Comparator") );
    }

    if (metaNode.Contains("CompareValueFloat"))
    {
        p_floatInput->SetValue( metaNode.Get<float>("CompareValueFloat") );
    }

    UpdateFromVariable();
}

void ASMCTransitionConditionInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("VariableName",      p_varNameInput->GetSelectedLabel());
    metaNode->Set("Comparator",        p_comparatorInput->GetSelectedValue());
    metaNode->Set("CompareValueFloat", p_floatInput->GetValue());
}

