#include "BangEditor/ASMCTransitionConditionInput.h"

#include "Bang/MetaNode.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
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
    p_floatInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_varNameInput->GetGameObject()->SetParent(this);
    p_comparatorInput->GetGameObject()->SetParent(this);
    p_floatInput->GetGameObject()->SetParent(this);
}

ASMCTransitionConditionInput::~ASMCTransitionConditionInput()
{
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

void ASMCTransitionConditionInput::OnValueChanged(
                                    EventEmitter<IEventsValueChanged> *ee)
{
    EventEmitter<IEventsValueChanged>::PropagateToListeners(
                 &IEventsValueChanged::OnValueChanged, this);
}

void ASMCTransitionConditionInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if (metaNode.Contains("VariableName"))
    {
        String previousSelectedValue = p_varNameInput->GetSelectedLabel();
        String newSelectedVarName = metaNode.Get<String>("VariableName");

        p_varNameInput->SetSelectionByLabel(newSelectedVarName);
    }

    if (metaNode.Contains("VariableType"))
    {
        SetVariableType(
            metaNode.Get<AnimatorStateMachineVariable::Type>("VariableType") );
    }

    if (metaNode.Contains("Comparator"))
    {
        p_comparatorInput->SetSelectionByValue( metaNode.Get<uint>("Comparator") );
    }

    if (metaNode.Contains("CompareValueFloat"))
    {
        p_floatInput->SetValue( metaNode.Get<float>("CompareValueFloat") );
    }}

void ASMCTransitionConditionInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("VariableName",      p_varNameInput->GetSelectedLabel());
    metaNode->Set("Comparator",        p_comparatorInput->GetSelectedValue());
    metaNode->Set("CompareValueFloat", p_floatInput->GetValue());
}

