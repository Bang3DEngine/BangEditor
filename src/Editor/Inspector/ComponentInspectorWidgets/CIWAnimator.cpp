#include "BangEditor/CIWAnimator.h"

#include "Bang/Animator.h"
#include "Bang/Animation.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/UICheckBox.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIInputArray.h"
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

    p_animationsInput = GameObject::Create<UIInputArray>();
    p_animationsInput->SetGetNewElementFunction([this]()
    {
        return CreateAnimationEntry();
    });
    p_animationsInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    p_playOnStartInput = GameObjectFactory::CreateUICheckBox();
    p_playOnStartInput->EventEmitter<IEventsValueChanged>::RegisterListener(this);

    AddWidget("Play on start", p_playOnStartInput->GetGameObject());
    AddWidget(GameObjectFactory::CreateUIHSeparator(LayoutSizeType::PREFERRED, 5));
    AddLabel("Animations");
    AddWidget(p_animationsInput, -1);

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

    const Array<RH<Animation>>& animsRHs = animator->GetAnimations();

    // Add new ones
    for (uint i = p_animationsInput->Size(); i < animsRHs.Size(); ++i)
    {
        UIInputFileWithPreview *inputFile = CreateAnimationEntry();
        p_animationsInput->AddElement(inputFile);
    }

    // Update old and new ones
    const uint minSize = Math::Min(animsRHs.Size(), p_animationsInput->Size());
    for (uint i = 0; i < minSize; ++i)
    {
        Path animPath = Path::Empty;
        UIInputFileWithPreview *inputFile =
            SCAST<UIInputFileWithPreview*>(p_animationsInput->GetArray()[i]);
        if (Animation *animation = animsRHs[i].Get())
        {
            animPath = animation->GetResourceFilepath();
        }
        inputFile->SetPath(animPath);
    }

    // Remove if there are more
    for (uint i = animsRHs.Size(); i < p_animationsInput->Size(); ++i)
    {
        p_animationsInput->RemoveElement(i);
    }

    p_playOnStartInput->SetChecked( GetAnimator()->GetPlayOnStart() );
}

void CIWAnimator::OnValueChangedCIW(EventEmitter<IEventsValueChanged> *object)
{
    ComponentInspectorWidget::OnValueChangedCIW(object);

    Animator *animator = GetAnimator();

    if (object == p_animationsInput)
    {
        // Add or update
        for (uint i = 0; i < p_animationsInput->Size(); ++i)
        {
            UIInputFileWithPreview *inputFile =
                SCAST<UIInputFileWithPreview*>(p_animationsInput->GetArray()[i]);
            RH<Animation> animationRH =
                    Resources::Load<Animation>(inputFile->GetPath());
            if (i < animator->GetAnimations().Size())
            {
                animator->SetAnimation(i, animationRH.Get());
            }
            else
            {
                animator->AddAnimation(animationRH.Get(), i);
            }
        }

        // Remove
        for (uint i = p_animationsInput->Size();
             i < animator->GetAnimations().Size();
             ++i)
        {
            animator->RemoveAnimationByIndex(i);
        }
    }
    else
    {
        // Some animation has changed
        ASSERT(p_animationsInput->Size() == animator->GetAnimations().Size());
        for (uint i = 0; i < p_animationsInput->Size(); ++i)
        {
            GameObject *inputFileGo = p_animationsInput->GetArray()[i];
            auto inputFile = SCAST<UIInputFileWithPreview*>(inputFileGo);
            RH<Animation> animationRH = Resources::Load<Animation>(inputFile->GetPath());
            animator->SetAnimation(i, animationRH.Get());
        }

        if (object == p_playOnStartInput)
        {
            GetAnimator()->SetPlayOnStart( p_playOnStartInput->IsChecked() );
        }
    }
}

Animator *CIWAnimator::GetAnimator() const
{
    return SCAST<Animator*>( GetComponent() );
}

