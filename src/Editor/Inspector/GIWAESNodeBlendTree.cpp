#include "BangEditor/GIWAESNodeBlendTree.h"

#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineBlendTreeNode.h"
#include "Bang/Extensions.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/Assets.h"
#include "Bang/UIComboBox.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UIInputText.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/UIInputFile.h"

using namespace Bang;
using namespace BangEditor;

GIWAESNodeBlendTree::GIWAESNodeBlendTree()
{
}

GIWAESNodeBlendTree::~GIWAESNodeBlendTree()
{
}

void GIWAESNodeBlendTree::InitInnerWidgets()
{
    GIWAESNode::InitInnerWidgets();

    SetTitle("Animation State Blend Tree Node");
    SetName("GIWAESNodeBlendTree");

    p_nodeSecondAnimationInput = new UIInputFile();
    p_nodeSecondAnimationInput->SetExtensions(
        {Extensions::GetAnimationExtension()});
    p_nodeSecondAnimationInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_variableNameInput = GameObjectFactory::CreateUIComboBox();
    p_variableNameInput->EventEmitter<IEventsValueChanged>::RegisterListener(
        this);

    p_secondAnimationSpeedInput = GameObjectFactory::CreateUIInputNumber();
    p_secondAnimationSpeedInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    GetWidgetToLabel()
        .Get(p_nodeAnimationInput)
        ->GetText()
        ->SetContent("First Animation");
    AddWidget("Second Animation Speed",
              p_secondAnimationSpeedInput->GetGameObject());
    AddWidget("Second Animation", p_nodeSecondAnimationInput);
    AddWidget("Blend variable", p_variableNameInput->GetGameObject());

    SetLabelsWidth(140);
}

void GIWAESNodeBlendTree::UpdateFromReference()
{
    GIWAESNode::UpdateFromReference();

    const Array<String> &variableNamesInInput =
        p_variableNameInput->GetLabels();
    Array<String> asmVariableNames =
        GetAESNode()->GetSMNode()->GetStateMachine()->GetVariablesNames();

    if (asmVariableNames != variableNamesInInput)
    {
        p_variableNameInput->ClearItems();
        for (const String &varName : asmVariableNames)
        {
            p_variableNameInput->AddItem(varName, 0);
        }
    }

    if (AnimatorStateMachineBlendTreeNode *smNode =
            DCAST<AnimatorStateMachineBlendTreeNode *>(
                GetAESNode()->GetSMNode()))
    {
        p_nodeSecondAnimationInput->SetPath(
            smNode->GetSecondAnimation()
                ? smNode->GetSecondAnimation()->GetAssetFilepath()
                : Path::Empty());
        p_secondAnimationSpeedInput->SetValue(
            smNode->GetSecondAnimationSpeed());
    }
}

void GIWAESNodeBlendTree::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    GIWAESNode::OnValueChanged(ee);

    if (AnimatorStateMachineBlendTreeNode *smNode =
            DCAST<AnimatorStateMachineBlendTreeNode *>(
                GetAESNode()->GetSMNode()))
    {
        if (ee == p_nodeSecondAnimationInput)
        {
            smNode->SetSecondAnimation(
                Assets::Load<Animation>(
                    p_nodeSecondAnimationInput->GetPath())
                    .Get());
        }
        else if (ee == p_variableNameInput)
        {
            String selectedVarName = p_variableNameInput->GetSelectedLabel();
            smNode->SetBlendVariableName(selectedVarName);
        }
        else if (ee == p_secondAnimationSpeedInput)
        {
            smNode->SetSecondAnimationSpeed(
                p_secondAnimationSpeedInput->GetValue());
        }
    }
}

void GIWAESNodeBlendTree::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GIWAESNode::OnDestroyed(object);
}
