#include "BangEditor/AnimatorSMEditor.h"

#include "Bang/UILabel.h"
#include "Bang/Animator.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIDirLayoutMovableSeparator.h"

#include "BangEditor/AnimatorSMEditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AnimatorSMEditor::AnimatorSMEditor()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    UILayoutElement *mainLE = AddComponent<UILayoutElement>();
    mainLE->SetFlexibleSize(Vector2::One);

    UIHorizontalLayout *mainHL = AddComponent<UIHorizontalLayout>();
    mainHL->SetChildrenVerticalStretch(Stretch::FULL);

    UIFocusable *focusable = AddComponent<UIFocusable>();

    GameObject *animatorEditorSceneContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *nodesSceneLE = animatorEditorSceneContainer->
                                        AddComponent<UILayoutElement>();
        nodesSceneLE->SetMinSize( Vector2i(200) );
        nodesSceneLE->SetFlexibleSize( Vector2::One );

        p_animatorEditorScene = GameObject::Create<AnimatorSMEditorScene>();
        p_animatorEditorScene->SetParent(animatorEditorSceneContainer);
    }

    GameObject *inspectorContainer = GameObjectFactory::CreateUIGameObject();
    {
        UILayoutElement *inspectorLE = inspectorContainer->
                                       AddComponent<UILayoutElement>();
        inspectorLE->SetMinSize( Vector2i(200) );

        UIVerticalLayout *mainVL = inspectorContainer->
                                   AddComponent<UIVerticalLayout>();

        UILabel *varsLabel = GameObjectFactory::CreateUILabel();
        varsLabel->GetText()->SetContent("Variables");
        varsLabel->GetText()->SetTextColor(Color::Black);
        varsLabel->GetText()->SetHorizontalAlign(HorizontalAlignment::LEFT);
        varsLabel->GetGameObject()->SetParent(inspectorContainer);

        GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5)->
                           SetParent(inspectorContainer);

        GameObject *labeledGosContainer = GameObjectFactory::CreateUIGameObject();
        UIVerticalLayout *labeledGosVL = labeledGosContainer->
                                         AddComponent<UIVerticalLayout>();
        UILayoutElement *labeledGosLE =
                         labeledGosContainer->AddComponent<UILayoutElement>();
        labeledGosLE->SetFlexibleSize( Vector2(1, 9999) );
        labeledGosContainer->SetParent(inspectorContainer);

        UIImageRenderer *inspectorBG = inspectorContainer->
                                       AddComponent<UIImageRenderer>();
        inspectorBG->SetTint(Color::LightGray);
    }

    animatorEditorSceneContainer->SetParent(this);
    GameObjectFactory::CreateUIDirLayoutMovableHSeparator()->
                       GetGameObject()->SetParent(this);
    inspectorContainer->SetParent(this);
}

AnimatorSMEditor::~AnimatorSMEditor()
{
}

void AnimatorSMEditor::Update()
{
    GameObject::Update();
}

void AnimatorSMEditor::SetAnimator(Animator *animator)
{
    if (animator != p_animator)
    {
        if (GetAnimator())
        {
            GetAnimator()->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
        }

        p_animator = animator;

        if (GetAnimator())
        {
            GetAnimator()->EventEmitter<IEventsDestroy>::RegisterListener(this);
        }
    }
}

Animator* AnimatorSMEditor::GetAnimator() const
{
    return p_animator;
}

void AnimatorSMEditor::Clear()
{

}

void AnimatorSMEditor::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (object == GetAnimator())
    {
        Clear();
    }
}

