#include "BangEditor/ASMVariableInput.h"

#include "Bang/MetaNode.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputText.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/AnimatorStateMachineVariable.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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
    varNameGoLE->SetFlexibleSize(Vector2::One);
    varNameGoLE->SetMinHeight(20);
    varNameGoLE->SetMinWidth(100);

    p_varNameInput = GameObjectFactory::CreateUIInputText();
    p_varNameInput->GetText()->SetContent("NewVariable");
    p_varNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_varTypeInput = GameObjectFactory::CreateUIComboBox();
    p_varTypeInput->AddItem("Float", SCAST<uint>(AnimatorStateMachineVariable::
                                                 Type::FLOAT));
    p_varTypeInput->AddItem("Bool", SCAST<uint>(AnimatorStateMachineVariable::
                                                 Type::BOOL));
    p_varTypeInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_floatInput = GameObjectFactory::CreateUIInputNumber();
    UILayoutElement *floatLE = p_floatInput->GetGameObject()->
                               AddComponent<UILayoutElement>();
    floatLE->SetMinWidth(100);
    p_floatInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_boolInput = GameObjectFactory::CreateUICheckBox();
    p_boolInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    varNameGo->SetParent(this);
    p_varNameInput->GetGameObject()->SetParent(varNameGo);
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::MIN, 5.0f)->
                       SetParent(this);
    p_varTypeInput->GetGameObject()->SetParent(this);
    GameObjectFactory::CreateUIHSpacer(LayoutSizeType::FLEXIBLE, 999.0f)->
                       SetParent(this);
    p_floatInput->GetGameObject()->SetParent(this);
    p_boolInput->GetGameObject()->SetParent(this);

    SetVarType( AnimatorStateMachineVariable::Type::FLOAT );
}

ASMVariableInput::~ASMVariableInput()
{
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
                p_boolInput->GetGameObject()->SetEnabled(true);
            break;

            case AnimatorStateMachineVariable::Type::FLOAT:
                p_floatInput->GetGameObject()->SetEnabled(true);
            break;
        }
        p_varTypeInput->SetSelectionByValue( SCAST<uint>(GetVarType()) );
    }
}

AnimatorStateMachineVariable::Type ASMVariableInput::GetVarType() const
{
    return m_varType;
}

void ASMVariableInput::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    if (ee == p_varTypeInput)
    {
        SetVarType(
            SCAST<AnimatorStateMachineVariable::Type>(
                                        p_varTypeInput->GetSelectedValue()));
    }
}

void ASMVariableInput::ImportMeta(const MetaNode &metaNode)
{
    GameObject::ImportMeta(metaNode);

    if (metaNode.Contains("VariableName"))
    {
        p_varNameInput->GetText()->SetContent(
                    metaNode.Get<String>("VariableName") );
    }

    if (metaNode.Contains("VariableType"))
    {
        p_varTypeInput->SetSelectionByValue( metaNode.Get<uint>("VariableType") );
    }

    if (metaNode.Contains("ValueFloat"))
    {
        p_floatInput->SetValue( metaNode.Get<float>("ValueFloat") );
    }

    if (metaNode.Contains("ValueBool"))
    {
        p_boolInput->SetChecked( metaNode.Get<bool>("ValueBool") );
    }
}

void ASMVariableInput::ExportMeta(MetaNode *metaNode) const
{
    GameObject::ExportMeta(metaNode);

    metaNode->Set("VariableName", p_varNameInput->GetText()->GetContent());
    metaNode->Set("VariableType", p_varTypeInput->GetSelectedValue());
    metaNode->Set("ValueFloat",   p_floatInput->GetValue());
    metaNode->Set("ValueBool",    p_boolInput->IsChecked());
}
