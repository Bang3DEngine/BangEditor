#include "BangEditor/GIWAESConnectionLine.h"

#include "Bang/Assert.h"
#include "Bang/UIList.h"
#include "Bang/UILabel.h"
#include "Bang/MetaNode.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachineConnection.h"

#include "BangEditor/AESNode.h"
#include "BangEditor/Inspector.h"
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

    p_transitionsList = GameObjectFactory::CreateUIList(true);
    p_transitionsList->SetSelectionCallback(
    [this](GameObject *item, UIList::Action action)
    {
        switch (action)
        {
            case UIList::Action::PRESSED:
            case UIList::Action::SELECTION_IN:
            break;

            default:
            break;
        }
    });
    p_transitionsList->GetScrollPanel()->SetVerticalShowScrollMode(
                                            ShowScrollMode::WHEN_NEEDED);
    p_transitionsList->GetScrollPanel()->SetForceHorizontalFit(true);
    UILayoutElement *listLE = p_transitionsList->GetGameObject()->
                              AddComponent<UILayoutElement>();
    listLE->SetFlexibleWidth(1.0f);

    p_transitionsListSeparator = GameObjectFactory::CreateUIHSeparator();

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

    AddWidget(p_transitionsList->GetGameObject(), 70);
    AddWidget(p_transitionsListSeparator, 20);
    AddLabel("Conditions");
    AddWidget(p_transitionConditionsInput, -1);
    AddWidget(p_notificationLabel->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 20);

    SetLabelsWidth(95);
    UpdateFromReference();
}

void GIWAESConnectionLine::SetAESConnectionLine(AESConnectionLine *connLine)
{
    ASSERT(!GetAESConnectionLine());
    p_aesConnectionLine = connLine;
    ASSERT(p_aesConnectionLine);

    p_aesConnectionLine->EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_aesConnectionLine->GetAESNodeTo()->
                         EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_aesConnectionLine->GetAESNodeFrom()->
                         EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_aesConnectionLine->GetAESNodeFrom()->GetSMNode()->
                         EventEmitter<IEventsDestroy>::RegisterListener(this);
}

AESConnectionLine *GIWAESConnectionLine::GetAESConnectionLine() const
{
    return p_aesConnectionLine;
}

void GIWAESConnectionLine::EnableNeededWidgets()
{
    if (GetAESConnectionLine())
    {
        bool moreThanOneTransition = GetAESConnectionLine()->
                                     GetSMConnections().Size() >= 2;
        SetWidgetEnabled(p_transitionsList->GetGameObject(),
                         moreThanOneTransition);
        SetWidgetEnabled(p_transitionsListSeparator, moreThanOneTransition);

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
    Array<AnimatorStateMachineConnection*> smConns = GetAESConnectionLine()->
                                                     GetSMConnections();
    if (p_transitionsList->GetNumItems() != smConns.Size())
    {
        p_transitionsList->Clear();

        uint i = 0;
        auto *toSMNode   = GetAESConnectionLine()->GetAESNodeTo()->GetSMNode();
        auto *fromSMNode = GetAESConnectionLine()->GetAESNodeFrom()->GetSMNode();
        for (AnimatorStateMachineConnection *smConn : smConns)
        {
            BANG_UNUSED(smConn);
            GameObject *listItemGo = GameObjectFactory::CreateUIGameObject();
            listItemGo->AddComponent<UIFocusable>();
            UILabel *listItemLabel = GameObjectFactory::CreateUILabel();
            listItemLabel->GetText()->SetContent(
                        "Transition " + String::ToString(i) + " from " +
                        fromSMNode->GetName() + " -> " + toSMNode->GetName());
            UILayoutElement *itemLE = listItemGo->AddComponent<UILayoutElement>();
            itemLE->SetMinHeight(30);
            listItemLabel->GetGameObject()->SetParent(listItemGo);
            p_transitionsList->AddItem(listItemGo);

            p_transitionsList->SetSelection(0);
            ++i;
        }
    }

    if (p_selectedSMConnection)
    {
        p_transitionConditionsInput->UpdateRows(
                    p_selectedSMConnection->GetTransitionConditions() );

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
        p_transitionConditionsInput->UpdateRows(
                    p_selectedSMConnection->GetTransitionConditions() );
    }
    EnableNeededWidgets();
}

void GIWAESConnectionLine::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    BANG_UNUSED(ee);
    if (p_selectedSMConnection)
    {
        p_transitionConditionsInput->UpdateReferences<ASMCTransitionCondition>(
                p_selectedSMConnection->GetTransitionConditions(),
                [this]()
                {
                    auto smConnection = p_selectedSMConnection;
                    ASMCTransitionCondition *transCond =
                                smConnection->CreateAndAddTransitionCondition();
                    return transCond;
                },
                [this](ASMCTransitionCondition *transitionCond)
                {
                    auto smConnection = p_selectedSMConnection;
                    smConnection->RemoveTransitionCondition(transitionCond);
                });
    }
}

void GIWAESConnectionLine::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    Inspector::GetActive()->Clear();
}

