#include "BangEditor/ASMVariableInput.h"

#include <sys/types.h>
#include <ostream>

#include "Bang/Animator.h"
#include "Bang/AnimatorStateMachineVariable.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/MetaNode.h"
#include "Bang/MetaNode.tcc"
#include "Bang/Serializable.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/Editor.h"

using namespace Bang;
using namespace BangEditor;

ASMVariableInput::ASMVariableInput()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    hl->SetSpacing(5);

    UILayoutElement *le = AddComponent<UILayoutElement>();
    le->SetMinHeight(20);
    le->SetFlexibleWidth(1.0f);

    GameObject *varNameGo = GameObjectFactory::CreateUIGameObject();
    varNameGo->AddComponent<UIVerticalLayout>();
    UILayoutElement *varNameGoLE = varNameGo->AddComponent<UILayoutElement>();
    varNameGoLE->SetFlexibleSize(Vector2::One());
    varNameGoLE->SetMinHeight(20);
    varNameGoLE->SetMinWidth(40);

    p_varNameInput = GameObjectFactory::CreateUIInputText();
    p_varNameInput->GetText()->SetContent("New Variable Name");
    p_varNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_varTypeInput = GameObjectFactory::CreateUIComboBox();
    p_varTypeInput->AddItem(
        "Float", SCAST<uint>(AnimatorStateMachineVariable::Type::FLOAT));
    p_varTypeInput->AddItem(
        "Bool", SCAST<uint>(AnimatorStateMachineVariable::Type::BOOL));
    p_varTypeInput->AddItem(
        "Trigger", SCAST<uint>(AnimatorStateMachineVariable::Type::TRIGGER));
    p_varTypeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    GameObject *varInputGo = GameObjectFactory::CreateUIGameObject();
    varInputGo->AddComponent<UIHorizontalLayout>();
    UILayoutElement *varInputLE = varInputGo->AddComponent<UILayoutElement>();
    varInputLE->SetFlexibleWidth(1.0f);
    varInputLE->SetMinWidth(40);

    p_floatInput = GameObjectFactory::CreateUIInputNumber();
    p_floatInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_boolInput = GameObjectFactory::CreateUICheckBox();
    p_boolInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    varNameGo->SetParent(this);
    p_varNameInput->GetGameObject()->SetParent(varNameGo);
    p_varTypeInput->GetGameObject()->SetParent(this);
    varInputGo->SetParent(this);
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 0.001f)
        ->SetParent(varInputGo);
    p_floatInput->GetGameObject()->SetParent(varInputGo);
    p_boolInput->GetGameObject()->SetParent(varInputGo);

    SetVarType(AnimatorStateMachineVariable::Type::FLOAT);
}

ASMVariableInput::~ASMVariableInput()
{
}

Animator *ASMVariableInput::GetSelectedAnimator() const
{
    GameObject *selectedGameObject =
        Editor::GetInstance()->GetSelectedGameObject();
    Animator *selectedAnimator =
        selectedGameObject ? selectedGameObject->GetComponent<Animator>()
                           : nullptr;
    return selectedAnimator;
}

void ASMVariableInput::SetVarType(AnimatorStateMachineVariable::Type type)
{
    if (type != GetVarType())
    {
        p_boolInput->GetGameObject()->SetEnabled(false);
        p_floatInput->GetGameObject()->SetEnabled(false);
        m_varType = type;

        switch (GetVarType())
        {
            case AnimatorStateMachineVariable::Type::BOOL:
            case AnimatorStateMachineVariable::Type::TRIGGER:
                p_boolInput->GetGameObject()->SetEnabled(true);
                break;

            case AnimatorStateMachineVariable::Type::FLOAT:
                p_floatInput->GetGameObject()->SetEnabled(true);
                break;

            default: break;
        }
        p_varTypeInput->SetSelectionByValue(SCAST<uint>(GetVarType()));
    }
}

AnimatorStateMachineVariable::Type ASMVariableInput::GetVarType() const
{
    return m_varType;
}

void ASMVariableInput::Update()
{
    GameObject::Update();
}

void ASMVariableInput::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    if (ee == p_varTypeInput)
    {
        SetVarType(SCAST<AnimatorStateMachineVariable::Type>(
            p_varTypeInput->GetSelectedValue()));
    }

    EventEmitter<IEventsValueChanged>::PropagateToListeners(
        &IEventsValueChanged::OnValueChanged, this);
}

void ASMVariableInput::ImportMeta(const MetaNode &metaNode)
{
    Serializable::ImportMeta(metaNode);

    if (metaNode.Contains("VariableName"))
    {
        if (p_varNameInput->GetText())
        {
            p_varNameInput->GetText()->SetContent(
                metaNode.Get<String>("VariableName"));
        }
    }

    if (metaNode.Contains("VariableType"))
    {
        p_varTypeInput->SetSelectionByValue(metaNode.Get<uint>("VariableType"));
    }

    Variant defaultValueVariant;
    if (metaNode.Contains("Variant"))
    {
        defaultValueVariant = metaNode.Get<Variant>("Variant");
    }

    if (Animator *selectedAnimator = GetSelectedAnimator())
    {
        String varName = p_varNameInput->GetText()->GetContent();
        p_boolInput->SetChecked(selectedAnimator->GetVariableBool(varName));
        p_floatInput->SetValue(selectedAnimator->GetVariableFloat(varName));
    }
    else
    {
        p_boolInput->SetChecked(defaultValueVariant.GetBool());
        p_floatInput->SetValue(defaultValueVariant.GetFloat());
    }
}

void ASMVariableInput::ExportMeta(MetaNode *metaNode) const
{
    Serializable::ExportMeta(metaNode);

    metaNode->Set("VariableName", p_varNameInput->GetText()->GetContent());
    metaNode->Set("VariableType", p_varTypeInput->GetSelectedValue());

    Variant defaultValueVariant;
    defaultValueVariant.SetFloat(p_floatInput->GetValue());
    defaultValueVariant.SetBool(p_boolInput->IsChecked());
    defaultValueVariant.SetType(
        SCAST<Variant::Type>(p_varTypeInput->GetSelectedValue()));
    metaNode->Set("Variant", defaultValueVariant);
}
