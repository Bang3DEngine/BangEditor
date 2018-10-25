#include "BangEditor/AnimatorSMEditor.h"

#include "Bang/Alignment.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineLayer.h"
#include "Bang/AnimatorStateMachineVariable.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/Path.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Stretch.h"
#include "Bang/UIButton.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILabel.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/ASMLayerInput.h"
#include "BangEditor/ASMVariableInput.h"
#include "BangEditor/AnimatorSMEditorScene.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Explorer.h"
#include "BangEditor/UIInputArray.h"

using namespace Bang;
using namespace BangEditor;

AnimatorSMEditor::AnimatorSMEditor()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIVerticalLayout>();
    UILayoutElement *mainLE = AddComponent<UILayoutElement>();
    mainLE->SetFlexibleSize(Vector2::One);

    GameObject *toolbar = GameObjectFactory::CreateUIGameObject();
    UIHorizontalLayout *toolbarHL = toolbar->AddComponent<UIHorizontalLayout>();
    toolbarHL->SetPaddings(5, 10, 5, 10);

    UILayoutElement *toolbarLE = toolbar->AddComponent<UILayoutElement>();
    toolbarLE->SetFlexibleSize(Vector2(1.0f, 0.0f));
    toolbarLE->SetMinHeight(40);

    p_centerSceneButton = GameObjectFactory::CreateUIButton(
        "", EditorTextureFactory::GetEyeIcon());
    p_centerSceneButton->GetIcon()->SetTint(Color::DarkGray);
    p_centerSceneButton->AddClickedCallback(
        [this]() { p_animatorEditorScene->CenterScene(); });
    p_centerSceneButton->GetGameObject()->SetParent(toolbar);

    GameObject *botHLGo = GameObjectFactory::CreateUIGameObject();
    UILayoutElement *botHLLE = botHLGo->AddComponent<UILayoutElement>();
    botHLLE->SetFlexibleSize(Vector2::One);

    UIHorizontalLayout *botHL = botHLGo->AddComponent<UIHorizontalLayout>();
    botHL->SetChildrenVerticalStretch(Stretch::FULL);

    AddComponent<UIFocusable>();

    GameObject *animatorEditorSceneContainer =
        GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *nodesSceneLE =
            animatorEditorSceneContainer->AddComponent<UILayoutElement>();
        nodesSceneLE->SetMinSize(Vector2i(200));
        nodesSceneLE->SetFlexibleSize(Vector2(3.0f, 1.0f));

        p_animatorEditorScene = new AnimatorSMEditorScene();
        p_animatorEditorScene->SetParent(animatorEditorSceneContainer);
    }

    GameObject *inspectorContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *inspectorLE =
            inspectorContainer->AddComponent<UILayoutElement>();
        inspectorLE->SetFlexibleSize(Vector2(0.5f, 1.0f));

        UIVerticalLayout *inspVL =
            inspectorContainer->AddComponent<UIVerticalLayout>();
        inspVL->SetPaddingLeft(10);

        UILabel *varsLabel = GameObjectFactory::CreateUILabel();
        varsLabel->GetText()->SetContent("Variables");
        varsLabel->GetText()->SetTextColor(Color::Black);
        varsLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        varsLabel->GetGameObject()->SetParent(inspectorContainer);
        varsLabel->GetGameObject()
            ->GetComponent<UILayoutElement>()
            ->SetFlexibleHeight(0.0f);

        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5)
            ->SetParent(inspectorContainer);

        p_variablesInput = new UIInputArray();
        p_variablesInput->SetCreateNewRowGameObjectFunction(
            []() { return new ASMVariableInput(); });
        p_variablesInput->EventEmitter<IEventsValueChanged>::RegisterListener(
            this);
        p_variablesInput->SetParent(inspectorContainer);

        UILayoutElement *varsLE =
            p_variablesInput->AddComponent<UILayoutElement>();
        varsLE->SetFlexibleSize(Vector2(1.0f));

        GameObjectFactory::CreateUIVSpacer(LayoutSizeType::PREFERRED, 20)
            ->SetParent(inspectorContainer);

        UILabel *layersLabel = GameObjectFactory::CreateUILabel();
        layersLabel->GetText()->SetContent("Layers");
        layersLabel->GetText()->SetTextColor(Color::Black);
        layersLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        layersLabel->GetGameObject()->SetParent(inspectorContainer);
        layersLabel->GetGameObject()
            ->GetComponent<UILayoutElement>()
            ->SetFlexibleHeight(0.0f);

        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5)
            ->SetParent(inspectorContainer);

        p_layersInput = new UIInputArray();
        p_layersInput->SetCreateNewRowGameObjectFunction([this]() {
            ASMLayerInput *layerInput = new ASMLayerInput();
            layerInput->EventEmitter<IEventsASMLayerInput>::RegisterListener(
                this);

            return layerInput;
        });
        p_layersInput->EventEmitter<IEventsUIInputArray>::RegisterListener(
            this);
        p_layersInput->EventEmitter<IEventsValueChanged>::RegisterListener(
            this);
        p_layersInput->SetParent(inspectorContainer);

        UILayoutElement *layersLE =
            p_layersInput->AddComponent<UILayoutElement>();
        layersLE->SetFlexibleSize(Vector2(1.0f));

        UIImageRenderer *inspectorBG =
            inspectorContainer->AddComponent<UIImageRenderer>();
        inspectorBG->SetTint(Color::LightGray);
    }

    toolbar->SetParent(this);
    botHLGo->SetParent(this);
    animatorEditorSceneContainer->SetParent(botHLGo);
    // GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->
    //                    GetGameObject()->SetParent(botHLGo);
    GameObjectFactory::CreateUIVSeparator()->SetParent(botHLGo);
    inspectorContainer->SetParent(botHLGo);
}

AnimatorSMEditor::~AnimatorSMEditor()
{
}

void AnimatorSMEditor::Update()
{
    GameObject::Update();

    p_centerSceneButton->SetBlocked((p_animatorEditorScene == nullptr));

    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(
                Extensions::GetAnimatorStateMachineExtension()))
        {
            RH<AnimatorStateMachine> animSMRH =
                Resources::Load<AnimatorStateMachine>(selectedPath);
            SetAnimatorSM(animSMRH.Get());
        }
    }

    if (GetAnimatorSM())
    {
        AnimatorStateMachineLayer *currentSelectedLayer = nullptr;

        if (Time::GetPassedTimeSince(m_lastVariablesInputUpdateTime) >=
            Time::Seconds(0.2f))
        {
            p_layersInput->UpdateRows(GetAnimatorSM()->GetLayers());
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

        if (GetAnimatorSM())
        {
            if (GetAnimatorSM()->GetLayers().IsEmpty())
            {
                GetAnimatorSM()->CreateNewLayer();
            }

            p_layersInput->UpdateRows(GetAnimatorSM()->GetLayers());
            ASSERT(p_layersInput->GetRowGameObjects().Size() >= 1);

            // Select first layer
            SCAST<ASMLayerInput *>(p_layersInput->GetRowGameObjects()[0])
                ->Select();
        }
    }
}

AnimatorStateMachine *AnimatorSMEditor::GetAnimatorSM() const
{
    return p_animatorSM.Get();
}

void AnimatorSMEditor::Clear()
{
    p_animatorEditorScene->Clear();
    p_layersInput->Clear();
    p_variablesInput->Clear();
}

void AnimatorSMEditor::OnLayerInputSelected(ASMLayerInput *selectedLayerInput)
{
    for (GameObject *layerInputRow : p_layersInput->GetRowGameObjects())
    {
        ASMLayerInput *layerInput = DCAST<ASMLayerInput *>(layerInputRow);
        ASSERT(layerInput);

        if (layerInput != selectedLayerInput)
        {
            layerInput->UnSelect();
        }
    }

    uint selectedLayerIndex =
        p_layersInput->GetRowGameObjects().IndexOf(selectedLayerInput);
    ASSERT(selectedLayerIndex < GetAnimatorSM()->GetLayers().Size());
    p_animatorEditorScene->SetAnimatorSMLayer(
        GetAnimatorSM()->GetLayers()[selectedLayerIndex]);
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
            [this]() {
                ASSERT(GetAnimatorSM());
                auto newVar = GetAnimatorSM()->CreateNewVariable();
                return newVar;
            },
            [this](AnimatorStateMachineVariable *varToRemove) {
                ASSERT(GetAnimatorSM());
                GetAnimatorSM()->RemoveVariable(varToRemove);
            });
    }
    else if (ee == p_layersInput)
    {
        p_layersInput->UpdateReferences<AnimatorStateMachineLayer>(
            GetAnimatorSM()->GetLayers(),
            [this]() {
                ASSERT(GetAnimatorSM());
                return GetAnimatorSM()->CreateNewLayer();
            },
            [this](AnimatorStateMachineLayer *layerToRemove) {
                ASSERT(GetAnimatorSM());
                GetAnimatorSM()->RemoveLayer(layerToRemove);
            });
    }
}
