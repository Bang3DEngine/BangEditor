#include "BangEditor/GIWAESConnectionLine.h"

#include "Bang/Assert.h"
#include "Bang/UILabel.h"
#include "Bang/MetaNode.h"
#include "Bang/UITextRenderer.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachineConnection.h"

#include "BangEditor/UIInputArray.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/ASMCTransitionConditionInput.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GIWAESConnectionLine::GIWAESConnectionLine()
{
}

GIWAESConnectionLine::~GIWAESConnectionLine()
{
}

void GIWAESConnectionLine::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();

    SetTitle("Transition");
    SetName("GIWAESConnectionLine");

    p_transitionConditionsInput = GameObject::Create<UIInputArray>();
    p_transitionConditionsInput->SetCreateNewRowGameObjectFunction([]()
    {
        ASMCTransitionConditionInput *transitionConditionInput =
                            GameObject::Create<ASMCTransitionConditionInput>();
        return transitionConditionInput;
    });
    p_transitionConditionsInput->EventEmitter<IEventsValueChanged>::
                                 RegisterListener(this);

    p_notificationLabel = GameObjectFactory::CreateUILabel();
    p_notificationLabel->GetText()->SetContent("Create at least one variable to "
                                               "add transition conditions");
    p_notificationLabel->GetText()->SetTextColor(Color::Red);

    AddLabel("Conditions");
    AddWidget(p_transitionConditionsInput, -1);
    AddWidget(p_notificationLabel->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), -1);

    SetLabelsWidth(95);
    UpdateFromReference();
}

void GIWAESConnectionLine::SetAESConnectionLine(AESConnectionLine *connLine)
{
    ASSERT(!GetAESConnectionLine());
    p_aesConnectionLine = connLine;
    p_aesConnectionLine->EventEmitter<IEventsDestroy>::RegisterListener(this);
}

AESConnectionLine *GIWAESConnectionLine::GetAESConnectionLine() const
{
    return p_aesConnectionLine;
}

void GIWAESConnectionLine::EnableNeededWidgets()
{
    if (GetAESConnectionLine())
    {
        bool atLeastOneVar = (GetAESConnectionLine()->GetAnimatorSM()->
                              GetVariables().Size() >= 1);

        SetWidgetEnabled(p_transitionConditionsInput, atLeastOneVar);
        SetWidgetEnabled(p_notificationLabel->GetGameObject(), !atLeastOneVar);
    }
}

void GIWAESConnectionLine::UpdateFromReference()
{
    InspectorWidget::UpdateFromReference();

    Array<MetaNode> metaNodes;
    AnimatorStateMachineConnection *smConn = GetAESConnectionLine()->
                                             GetSMConnection();
    p_transitionConditionsInput->UpdateRows( smConn->GetTransitionConditions() );

    uint i = 0;
    for (GameObject *transCondInputGo :
                            p_transitionConditionsInput->GetRowGameObjects())
    {
        ASMCTransitionConditionInput *transCondInput =
                SCAST<ASMCTransitionConditionInput*>(transCondInputGo);
        transCondInput->SetAnimatorStateMachine(
                    GetAESConnectionLine()->GetAnimatorSM() );
        ++i;
    }
    p_transitionConditionsInput->UpdateRows( smConn->GetTransitionConditions() );

    EnableNeededWidgets();
}

void GIWAESConnectionLine::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    BANG_UNUSED(ee);
    p_transitionConditionsInput->UpdateReferences<ASMCTransitionCondition>(
            p_aesConnectionLine->GetSMConnection()->GetTransitionConditions(),
            [this]()
            {
                auto smConnection = p_aesConnectionLine->GetSMConnection();
                ASMCTransitionCondition *transCond =
                               smConnection->CreateAndAddTransitionCondition();
                return transCond;
            },
            [this](ASMCTransitionCondition *transitionCond)
            {
                auto smConnection = p_aesConnectionLine->GetSMConnection();
                smConnection->RemoveTransitionCondition(transitionCond);
            });
}

void GIWAESConnectionLine::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{

}

