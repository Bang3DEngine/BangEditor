#include "BangEditor/RIWAnimatorStateMachine.h"

#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/ResourceHandle.h"
#include "Bang/UIButton.h"
#include "Bang/UILayoutElement.h"
#include "BangEditor/AnimatorSMEditor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/RIWResource.tcc"

namespace Bang {
class IEventsValueChanged;
class Texture2D;
template <class > class EventEmitter;
}  // namespace Bang

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RIWAnimatorStateMachine::RIWAnimatorStateMachine()
{
}

RIWAnimatorStateMachine::~RIWAnimatorStateMachine()
{
}

void RIWAnimatorStateMachine::Init()
{
    RIWResource<AnimatorStateMachine>::Init();

    SetTitle("Animator State Machine");
    SetName("RIWAnimatorStateMachine");

    p_openInEditorButton = GameObjectFactory::CreateUIButton(
                                    "Open in AnimatorSM Editor");
    p_openInEditorButton->AddClickedCallback([]()
    {
        EditorScene *edScene = EditorSceneManager::GetEditorScene();
        edScene->OpenTab( edScene->GetAnimatorSMEditor() );
    });

    UILayoutElement *buttonLE = p_openInEditorButton->GetGameObject()->
                                 AddComponent<UILayoutElement>();
    buttonLE->SetFlexibleSize( Vector2::One );
    buttonLE->SetLayoutPriority(3);

    AddWidget(p_openInEditorButton->GetGameObject());
}

void RIWAnimatorStateMachine::UpdateInputsFromResource()
{
    // Empty
}

void RIWAnimatorStateMachine::OnValueChangedRIWResource(
                        EventEmitter<IEventsValueChanged> *object)
{
    BANG_UNUSED(object);
}

Texture2D *RIWAnimatorStateMachine::GetIconTexture() const
{
    return EditorTextureFactory::GetAnimatorSMIcon();
}

AnimatorStateMachine *RIWAnimatorStateMachine::GetAnimatorStateMachine() const
{
    return SCAST<AnimatorStateMachine*>( GetResource().Get() );
}

