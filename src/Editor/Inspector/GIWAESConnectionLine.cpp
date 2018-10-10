#include "BangEditor/GIWAESConnectionLine.h"

#include "Bang/Assert.h"
#include "Bang/MetaNode.h"
#include "Bang/GameObjectFactory.h"
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

void GIWAESConnectionLine::Init()
{
    InspectorWidget::Init();

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

    AddLabel("Conditions");
    AddWidget(p_transitionConditionsInput, -1);

    SetLabelsWidth(95);
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

void GIWAESConnectionLine::UpdateFromReference()
{
    InspectorWidget::UpdateFromReference();

    Array<MetaNode> metaNodes;
    AnimatorStateMachineConnection *smConn = GetAESConnectionLine()->
                                             GetSMConnection();
    p_transitionConditionsInput->UpdateRows( smConn->GetTransitionConditions() );
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

