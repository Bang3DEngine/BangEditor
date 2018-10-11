#include "BangEditor/AnimatorSMEditor.h"

#include "Bang/UILabel.h"
#include "Bang/Animator.h"
#include "Bang/MetaNode.h"
#include "Bang/UIButton.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIDirLayoutMovableSeparator.h"

#include "BangEditor/Explorer.h"
#include "BangEditor/UIInputArray.h"
#include "BangEditor/ASMVariableInput.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/AnimatorSMEditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AnimatorSMEditor::AnimatorSMEditor()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIVerticalLayout>();
    UILayoutElement *mainLE = AddComponent<UILayoutElement>();
    mainLE->SetFlexibleSize( Vector2::One );

    GameObject *toolbar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolbarHL = toolbar->AddComponent<UIHorizontalLayout>();
    toolbarHL->SetPaddings(5, 10, 5, 10);

    UILayoutElement *toolbarLE = toolbar->AddComponent<UILayoutElement>();
    toolbarLE->SetFlexibleSize(Vector2(1.0f, 0.0f));
    toolbarLE->SetMinHeight(40);

    p_centerSceneButton = GameObjectFactory::CreateUIButton(
                                "", EditorTextureFactory::GetEyeIcon());
    p_centerSceneButton->GetIcon()->SetTint(Color::DarkGray);
    p_centerSceneButton->AddClickedCallback([this]()
    {
       p_animatorEditorScene->CenterScene();
    });
    p_centerSceneButton->GetGameObject()->SetParent(toolbar);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *botHLLE = botHLGo->AddComponent<UILayoutElement>();
    botHLLE->SetFlexibleSize(Vector2::One);

    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    botHL->SetChildrenVerticalStretch(Stretch::FULL);

    AddComponent<UIFocusable>();

    GameObject *animatorEditorSceneContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *nodesSceneLE = animatorEditorSceneContainer->
                                        AddComponent<UILayoutElement>();
        nodesSceneLE->SetMinSize( Vector2i(200) );
        nodesSceneLE->SetFlexibleSize( Vector2(3.0f, 1.0f) );

        p_animatorEditorScene = GameObject::Create<AnimatorSMEditorScene>();
        p_animatorEditorScene->SetParent(animatorEditorSceneContainer);
    }

    GameObject *inspectorContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *inspectorLE = inspectorContainer->
                                       AddComponent<UILayoutElement>();
        inspectorLE->SetFlexibleSize( Vector2(0.5f, 1.0f) );

        inspectorContainer->AddComponent<UIVerticalLayout>();

        UILabel *varsLabel = GameObjectFactory::CreateUILabel();
        varsLabel->GetText()->SetContent("Variables");
        varsLabel->GetText()->SetTextColor(Color::Black);
        varsLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        varsLabel->GetGameObject()->SetParent(inspectorContainer);

        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5)->
                           SetParent(inspectorContainer);

        p_variablesInput = GameObject::Create<UIInputArray>();
        p_variablesInput->SetCreateNewRowGameObjectFunction([]()
        {
            ASMVariableInput *varInput = GameObject::Create<ASMVariableInput>();
            return varInput;
        });
        p_variablesInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);
        p_variablesInput->SetParent(inspectorContainer);

        UILayoutElement *varsLE = p_variablesInput->AddComponent<UILayoutElement>();
        varsLE->SetFlexibleSize(Vector2(9999.9f));

        UIImageRenderer *inspectorBG = inspectorContainer->
                                       AddComponent<UIImageRenderer>();
        inspectorBG->SetTint(Color::LightGray);
    }

    toolbar->SetParent(this);
    botHLGo->SetParent(this);
    animatorEditorSceneContainer->SetParent(botHLGo);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->
                       GetGameObject()->SetParent(botHLGo);
    inspectorContainer->SetParent(botHLGo);
}

AnimatorSMEditor::~AnimatorSMEditor()
{
}

void AnimatorSMEditor::Update()
{
    GameObject::Update();

    p_centerSceneButton->SetBlocked( (p_animatorEditorScene == nullptr) );

    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(
                            Extensions::GetAnimatorStateMachineExtension() ))
        {
            RH<AnimatorStateMachine> animSMRH =
                        Resources::Load<AnimatorStateMachine>(selectedPath);
            SetAnimatorSM(animSMRH.Get());
        }
    }

    if (GetAnimatorSM())
    {
        if (Time::GetPassedTimeSince(m_lastVariablesInputUpdateTime) >=
            Time::Seconds(0.2f))
        {
            p_variablesInput->UpdateRows(GetAnimatorSM()->GetVariables());
            m_lastVariablesInputUpdateTime = Time::GetNow();
        }
    }
}

void AnimatorSMEditor::SetAnimatorSM(AnimatorStateMachine *animatorSM)
{
    if (animatorSM != GetAnimatorSM())
    {
        Clear();

        p_animatorSM.Set(animatorSM);
        p_animatorEditorScene->SetAnimatorSM( GetAnimatorSM() );
    }
}

AnimatorStateMachine *AnimatorSMEditor::GetAnimatorSM() const
{
    return p_animatorSM.Get();
}

void AnimatorSMEditor::Clear()
{
    p_animatorEditorScene->Clear();
    p_variablesInput->Clear();
}

void AnimatorSMEditor::OnValueChanged(EventEmitter<IEventsValueChanged> *ee)
{
    if (!GetAnimatorSM())
    {
        return;
    }

    if (ee == p_variablesInput)
    {
        p_variablesInput->UpdateReferences<AnimatorStateMachineVariable>(
                        GetAnimatorSM()->GetVariables(),
                        [this]()
                        {
                            ASSERT(GetAnimatorSM());
                            auto newVar = GetAnimatorSM()->CreateNewVariable();
                            return newVar;
                        },
                        [this](AnimatorStateMachineVariable *varToRemove)
                        {
                                ASSERT(GetAnimatorSM());
                            GetAnimatorSM()->RemoveVariable(varToRemove);
                        }
                    );
    }
}
