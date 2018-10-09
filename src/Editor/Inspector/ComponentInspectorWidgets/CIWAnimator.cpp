#include "BangEditor/CIWAnimator.h"

#include "Bang/Animator.h"
#include "Bang/Animation.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputFileWithPreview.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

CIWAnimator::CIWAnimator()
{
}

CIWAnimator::~CIWAnimator()
{
}

void CIWAnimator::InitInnerWidgets()
{
    ComponentInspectorWidget::InitInnerWidgets();

    SetName("CIWAnimator");
    SetTitle("Animator");

    p_animatorSMInput = GameObject::Create<UIInputFileWithPreview>();
    p_animatorSMInput->SetExtensions(
                            {Extensions::GetAnimatorStateMachineExtension()} );
    p_animatorSMInput->SetZoomable(false);
    p_animatorSMInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_playOnStartInput = GameObjectFactory::CreateUICheckBox();
    p_playOnStartInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Play on start", p_playOnStartInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddWidget("State Machine", p_animatorSMInput);

    SetLabelsWidth(90);
}

UIInputFileWithPreview* CIWAnimator::CreateAnimationEntry()
{
    UIInputFileWithPreview *entry = GameObject::Create<UIInputFileWithPreview>();
    entry->SetExtensions( {Extensions::GetAnimationExtension()} );
    entry->EventEmitter<IEventsValueChanged>::RegisterListener(this);
    entry->SetZoomable(false);
    return entry;
}

void CIWAnimator::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();

    Animator *animator = GetAnimator();
    AnimatorStateMachine *sm = animator->GetStateMachine();

    p_animatorSMInput->SetPath(sm ? sm->GetResourceFilepath() : Path::Empty);
    p_playOnStartInput->SetChecked( animator->GetPlayOnStart() );
}

void CIWAnimator::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    Animator *animator = GetAnimator();

    if (object == p_animatorSMInput)
    {
        animator->SetStateMachine(
                    Resources::Load<AnimatorStateMachine>(
                                p_animatorSMInput->GetPath()).Get() );
    }

    if (object == p_playOnStartInput)
    {
        GetAnimator()->SetPlayOnStart( p_playOnStartInput->IsChecked() );
    }
}

Animator *CIWAnimator::GetAnimator() const
{
    return SCAST<Animator*>( GetComponent() );
}

