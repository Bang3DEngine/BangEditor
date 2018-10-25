#include "BangEditor/GIWAESConnectionLine.h"

#include <sys/types.h>

#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachineTransition.h"
#include "Bang/AnimatorStateMachineTransitionCondition.h"
#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/MetaNode.h"
#include "Bang/String.h"
#include "Bang/Time.h"
#include "Bang/UIButton.h"
#include "Bang/UICheckBox.h"
#include "Bang/UIDirLayout.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIInputNumber.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIList.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/UITextRenderer.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/ASMCTransitionConditionInput.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/UIInputArray.h"

namespace Bang
{
class IEventsDestroy;
}

using namespace Bang;
using namespace BangEditor;

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
        [this](GameObject *item, UIList::Action action) {
            switch (action)
            {
                case UIList::Action::PRESSED:
                case UIList::Action::SELECTION_IN:
                {
                    uint itemIdx = p_transitionsList->GetItems().IndexOf(item);
                    ASSERT(itemIdx != -1u);

                    auto transitionsToNodeTo =
                        GetSMNodeFrom()->GetTransitionsTo(GetSMNodeTo());
                    ASSERT(itemIdx < transitionsToNodeTo.Size());

                    AnimatorStateMachineTransition *selectedConn =
                        transitionsToNodeTo.At(itemIdx);
                    ASSERT(selectedConn);

                    SetSelectedSMTransition(selectedConn);
                }
                break;

                default: break;
            }
        });
    p_transitionsList->SetWideSelectionMode(false);
    p_transitionsList->GetScrollPanel()->SetVerticalShowScrollMode(
        ShowScrollMode::WHEN_NEEDED);
    p_transitionsList->GetScrollPanel()->SetForceHorizontalFit(true);
    UILayoutElement *listLE =
        p_transitionsList->GetGameObject()->AddComponent<UILayoutElement>();
    listLE->SetFlexibleWidth(1.0f);

    p_transitionsListSeparator = GameObjectFactory::CreateUIHSeparator();

    p_transitionConditionsInput = new UIInputArray();
    p_transitionConditionsInput
        ->SetFunctions<AnimatorStateMachineTransitionCondition>(
            []() {
                ASMTransitionConditionInput *transitionConditionInput =
                    new ASMTransitionConditionInput();
                return transitionConditionInput;
            },
            [this]() {
                auto smConnection = GetSelectedSMTransition();
                AnimatorStateMachineTransitionCondition *transCond =
                    smConnection->CreateAndAddTransitionCondition();
                return transCond;
            },
            [](uint, uint) {
                // No move
            },
            [this](AnimatorStateMachineTransitionCondition *transitionCond) {
                auto smConnection = GetSelectedSMTransition();
                smConnection->RemoveTransitionCondition(transitionCond);
            });
    p_transitionConditionsInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_notificationLabel = GameObjectFactory::CreateUILabel();
    p_notificationLabel->GetText()->SetContent(
        "Create at least one variable to "
        "add transition conditions");
    p_notificationLabel->GetText()->SetTextColor(Color::Red);

    p_immediateTransitionInput = GameObjectFactory::CreateUICheckBox();
    p_immediateTransitionInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_transitionDurationInput = GameObjectFactory::CreateUIInputNumber();
    p_transitionDurationInput->SetMinValue(0.0f);
    p_transitionDurationInput
        ->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget(p_transitionsList->GetGameObject(), 70);
    AddWidget(p_transitionsListSeparator, 20);
    AddWidget("Can do immediate transition",
              p_immediateTransitionInput->GetGameObject());
    AddWidget("Transition duration",
              p_transitionDurationInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 20);
    AddLabel("Conditions");
    AddWidget(p_transitionConditionsInput, -1);
    AddWidget(p_notificationLabel->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(), 20);

    SetLabelsWidth(120);
    UpdateFromReference();
}

void GIWAESConnectionLine::SetAESConnectionLine(AESConnectionLine *connLine)
{
    ASSERT(!GetAESConnectionLine());
    p_aesConnectionLine = connLine;
    ASSERT(p_aesConnectionLine);

    p_aesConnectionLine->EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_aesConnectionLine->GetAESNodeTo()
        ->EventEmitter<IEventsDestroy>::RegisterListener(this);
    p_aesConnectionLine->GetAESNodeFrom()
        ->EventEmitter<IEventsDestroy>::RegisterListener(this);
    GetSMNodeFrom()->EventEmitter<IEventsDestroy>::RegisterListener(this);
    GetSMNodeTo()->EventEmitter<IEventsDestroy>::RegisterListener(this);

    const Array<AnimatorStateMachineTransition *> smTransitions =
        p_aesConnectionLine->GetSMTransitions();
    for (AnimatorStateMachineTransition *smTransition : smTransitions)
    {
        smTransition->EventEmitter<IEventsDestroy>::RegisterListener(this);
    }

    ASSERT(smTransitions.Size() >= 1);
    SetSelectedSMTransition(smTransitions[0]);
}

AESConnectionLine *GIWAESConnectionLine::GetAESConnectionLine() const
{
    return p_aesConnectionLine;
}

void GIWAESConnectionLine::EnableNeededWidgets()
{
    if (GetAESConnectionLine())
    {
        bool moreThanOneTransition =
            GetAESConnectionLine()->GetSMTransitions().Size() >= 2;
        SetWidgetEnabled(p_transitionsList->GetGameObject(),
                         moreThanOneTransition);
        SetWidgetEnabled(p_transitionsListSeparator, moreThanOneTransition);

        bool atLeastOneVar =
            GetSelectedSMTransition() &&
            (GetAESConnectionLine()->GetAnimatorSM()->GetVariables().Size() >=
             1);

        SetWidgetEnabled(p_transitionConditionsInput, atLeastOneVar);
        SetWidgetEnabled(p_notificationLabel->GetGameObject(), !atLeastOneVar);
    }
}

void GIWAESConnectionLine::SetSelectedSMTransition(
    AnimatorStateMachineTransition *connection)
{
    if (connection != GetSelectedSMTransition())
    {
        p_selectedSMTransition = connection;
    }
}

AnimatorStateMachineNode *GIWAESConnectionLine::GetSMNodeTo() const
{
    return GetAESConnectionLine()->GetAESNodeTo()->GetSMNode();
}

AnimatorStateMachineNode *GIWAESConnectionLine::GetSMNodeFrom() const
{
    return GetAESConnectionLine()->GetAESNodeFrom()->GetSMNode();
}

AnimatorStateMachineTransition *GIWAESConnectionLine::GetSelectedSMTransition()
    const
{
    return p_selectedSMTransition;
}

void GIWAESConnectionLine::UpdateFromReference()
{
    InspectorWidget::UpdateFromReference();

    Array<MetaNode> metaNodes;
    Array<AnimatorStateMachineTransition *> smConns =
        GetAESConnectionLine()->GetSMTransitions();
    if (p_transitionsList->GetNumItems() != smConns.Size())
    {
        p_transitionsList->Clear();

        uint i = 0;
        for (AnimatorStateMachineTransition *smConn : smConns)
        {
            BANG_UNUSED(smConn);
            GameObject *listItemGo = GameObjectFactory::CreateUIGameObject();
            listItemGo->AddComponent<UIFocusable>();
            auto hl = listItemGo->AddComponent<UIHorizontalLayout>();
            hl->SetPaddingRight(10);
            hl->SetPaddingLeft(10);
            hl->SetPaddingTop(5);
            hl->SetPaddingBot(5);
            hl->SetSpacing(10);

            UIButton *removeButton = GameObjectFactory::CreateUIButton();
            removeButton->SetIcon(EditorTextureFactory::GetLessIcon(),
                                  Vector2i(12));
            removeButton->GetDirLayout()->SetPaddings(5, 0, 5, 0);
            removeButton->GetIcon()->SetTint(Color::Red);
            removeButton->AddClickedCallback([this, smConn]() {
                GetSMNodeFrom()->RemoveTransition(smConn);
            });

            UILabel *listItemLabel = GameObjectFactory::CreateUILabel();
            listItemLabel->GetFocusable()->SetEnabled(false);
            listItemLabel->GetText()->SetContent(
                "Transition " + String::ToString(i) + " from " +
                GetSMNodeFrom()->GetName() + " -> " + GetSMNodeTo()->GetName());

            UILayoutElement *itemLE =
                listItemGo->AddComponent<UILayoutElement>();
            itemLE->SetMinHeight(30);

            listItemLabel->GetGameObject()->SetParent(listItemGo);
            removeButton->GetGameObject()->SetParent(listItemGo);
            p_transitionsList->AddItem(listItemGo);

            p_transitionsList->SetSelection(0);
            ++i;
        }
    }

    if (GetSelectedSMTransition())
    {
        p_immediateTransitionInput->SetChecked(
            GetSelectedSMTransition()->GetImmediateTransition());

        p_transitionDurationInput->SetValue(
            GetSelectedSMTransition()->GetTransitionDuration().GetSeconds());

        p_transitionConditionsInput->UpdateRows(
            GetSelectedSMTransition()->GetTransitionConditions());

        uint i = 0;
        for (GameObject *transCondInputGo :
             p_transitionConditionsInput->GetRowGameObjects())
        {
            ASMTransitionConditionInput *transCondInput =
                SCAST<ASMTransitionConditionInput *>(transCondInputGo);
            transCondInput->SetStateMachineTransition(
                GetSelectedSMTransition());
            ++i;
        }
        p_transitionConditionsInput->UpdateRows(
            GetSelectedSMTransition()->GetTransitionConditions());
    }
    EnableNeededWidgets();
}

void GIWAESConnectionLine::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    BANG_UNUSED(ee);
    if (GetSelectedSMTransition())
    {
        if (ee == p_immediateTransitionInput)
        {
            GetSelectedSMTransition()->SetImmediateTransition(
                p_immediateTransitionInput->IsChecked());
        }

        if (ee == p_transitionDurationInput)
        {
            GetSelectedSMTransition()->SetTransitionDuration(
                Time::Seconds(p_transitionDurationInput->GetValue()));
        }

        p_transitionConditionsInput
            ->UpdateReferences<AnimatorStateMachineTransitionCondition>(
                GetSelectedSMTransition()->GetTransitionConditions());
    }
}

void GIWAESConnectionLine::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (object == GetSelectedSMTransition())
    {
        SetSelectedSMTransition(nullptr);
    }
    else
    {
        GameObject::OnDestroyed(object);
    }
}
