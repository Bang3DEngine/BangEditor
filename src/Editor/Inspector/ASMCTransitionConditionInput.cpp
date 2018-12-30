#include "BangEditor/ASMCTransitionConditionInput.h"

#include <sys/types.h>
#include <ostream>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineLayer.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachineTransition.h"
#include "Bang/AnimatorStateMachineTransitionCondition.h"
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

ASMTransitionConditionInput::ASMTransitionConditionInput()
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

ASMTransitionConditionInput::~ASMTransitionConditionInput()
{
}

void ASMTransitionConditionInput::BeforeRender()
{
    GameObject::BeforeRender();

    if (GetStateMachineLayer())
    {
        UpdateFromVariable();
        p_varNameInput->SetSelectionByLabel(m_selectedVarName);
    }
}

void ASMTransitionConditionInput::SetVariableType(
    AnimatorStateMachineVariable::Type type)
{
    if (type != m_varType)
    {
        m_varType = type;

        p_comparatorInput->ClearItems();
        switch (m_varType)
        {
            case AnimatorStateMachineVariable::Type::BOOL:
            case AnimatorStateMachineVariable::Type::TRIGGER:
                p_floatInput->GetGameObject()->SetEnabled(false);
                p_comparatorInput->GetGameObject()->SetEnabled(true);
                p_comparatorInput->AddItem(
                    "True",
                    SCAST<uint>(AnimatorStateMachineTransitionCondition::
                                    Comparator::IS_TRUE));
                p_comparatorInput->AddItem(
                    "False",
                    SCAST<uint>(AnimatorStateMachineTransitionCondition::
                                    Comparator::IS_FALSE));

                break;

            case AnimatorStateMachineVariable::Type::FLOAT:
                p_floatInput->GetGameObject()->SetEnabled(true);
                p_comparatorInput->GetGameObject()->SetEnabled(true);
                p_comparatorInput->AddItem(
                    "Greater",
                    SCAST<uint>(AnimatorStateMachineTransitionCondition::
                                    Comparator::GREATER));
                p_comparatorInput->AddItem(
                    "Less",
                    SCAST<uint>(AnimatorStateMachineTransitionCondition::
                                    Comparator::LESS));
                break;

            default: break;
        }
    }
}

void ASMTransitionConditionInput::SetStateMachineTransition(
    AnimatorStateMachineTransition *transition)
{
    if (transition != p_stateMachineTransition)
    {
        p_stateMachineTransition = transition;
        GetStateMachineTransition()
            ->EventEmitter<IEventsDestroy>::RegisterListener(this);
        UpdateFromVariable();
    }
}

AnimatorStateMachine *ASMTransitionConditionInput::GetStateMachine() const
{
    return GetStateMachineLayer() ? GetStateMachineLayer()->GetStateMachine()
                                  : nullptr;
}

AnimatorStateMachineLayer *ASMTransitionConditionInput::GetStateMachineLayer()
    const
{
    return GetStateMachineNode() ? GetStateMachineNode()->GetLayer() : nullptr;
}

AnimatorStateMachineNode *ASMTransitionConditionInput::GetStateMachineNode()
    const
{
    return GetStateMachineTransition()
               ? GetStateMachineTransition()->GetNodeFrom()
               : nullptr;
}

AnimatorStateMachineTransition *
ASMTransitionConditionInput::GetStateMachineTransition() const
{
    return p_stateMachineTransition;
}

void ASMTransitionConditionInput::UpdateFromVariable()
{
    if (GetStateMachineLayer() && !m_updatingFromVariable)
    {
        m_updatingFromVariable = true;

        bool updateVariablesComboBox = false;
        updateVariablesComboBox |=
            (GetStateMachine()->GetVariableDefaults().Size() !=
             p_varNameInput->GetNumItems());
        if (!updateVariablesComboBox)
        {
            const auto &refVars = GetStateMachine()->GetVariableDefaults();
            const auto &comboLabels = p_varNameInput->GetLabels();
            for (uint i = 0; i < refVars.Size(); ++i)
            {
                if (refVars[i]->GetName() != comboLabels[i])
                {
                    updateVariablesComboBox = true;
                    break;
                }
            }
        }

        if (updateVariablesComboBox)
        {
            p_varNameInput->ClearItems();
            for (AnimatorStateMachineVariable *var :
                 GetStateMachine()->GetVariableDefaults())
            {
                p_varNameInput->AddItem(var->GetName(), 0);
            }
        }

        if (AnimatorStateMachineVariable *var =
                GetStateMachine()->GetVariableDefault(m_selectedVarName))
        {
            SetVariableType(var->GetType());
        }

        m_updatingFromVariable = false;
    }
}

void ASMTransitionConditionInput::OnDestroyed(EventEmitter<IEventsDestroy> *)
{
    GameObject::Destroy(this);
}

void ASMTransitionConditionInput::OnValueChanged(
    EventEmitter<IEventsValueChanged> *ee)
{
    if (ee == p_varNameInput)
    {
        m_selectedVarName = p_varNameInput->GetSelectedLabel();
    }
    UpdateFromVariable();

    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void ASMTransitionConditionInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if (metaNode.Contains("VariableName"))
    {
        m_selectedVarName = metaNode.Get<String>("VariableName");
        p_varNameInput->SetSelectionByLabel(m_selectedVarName);
    }

    if (metaNode.Contains("Comparator"))
    {
        p_comparatorInput->SetSelectionByValue(
            metaNode.Get<uint>("Comparator"));
    }

    if (metaNode.Contains("CompareValueFloat"))
    {
        p_floatInput->SetValue(metaNode.Get<float>("CompareValueFloat"));
    }

    UpdateFromVariable();
}

void ASMTransitionConditionInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("VariableName", p_varNameInput->GetSelectedLabel());
    metaNode->Set("Comparator", p_comparatorInput->GetSelectedValue());
    metaNode->Set("CompareValueFloat", p_floatInput->GetValue());
}
