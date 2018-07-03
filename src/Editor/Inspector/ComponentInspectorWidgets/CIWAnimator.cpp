#include "BangEditor/CIWAnimator.h"

#include "Bang/Animator.h"
#include "Bang/Extensions.h"

#include "BangEditor/UIInputFile.h"

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

    p_animationInput = GameObject::Create<UIInputFile>();
    p_animationInput->SetExtensions( {Extensions::GetAnimationExtension()} );
    p_animationInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Animation", p_animationInput);

    SetLabelsWidth(75);
}

void CIWAnimator::UpdateFromReference()
{
    ComponentInspectorWidget::UpdateFromReference();
    Animator *animator = GetAnimator();

}

void CIWAnimator::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);
}

Animator *CIWAnimator::GetAnimator() const
{
    return SCAST<Animator*>( GetComponent() );
}

