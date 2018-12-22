#include "BangEditor/AIWAnimatorStateMachine.h"

#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/AssetHandle.h"
#include "Bang/UIButton.h"
#include "Bang/UILayoutElement.h"
#include "BangEditor/AnimatorSMEditor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/AIWAsset.tcc"

namespace Bang
{
class IEventsValueChanged;
class Texture2D;
template <class>
class EventEmitter;
}

using namespace Bang;
using namespace BangEditor;

AIWAnimatorStateMachine::AIWAnimatorStateMachine()
{
}

AIWAnimatorStateMachine::~AIWAnimatorStateMachine()
{
}

void AIWAnimatorStateMachine::Init()
{
    AIWAsset<AnimatorStateMachine>::Init();

    SetTitle("Animator State Machine");
    SetName("AIWAnimatorStateMachine");

    p_openInEditorButton =
        GameObjectFactory::CreateUIButton("Open in AnimatorSM Editor");
    p_openInEditorButton->AddClickedCallback([]() {
        EditorScene *edScene = EditorSceneManager::GetEditorScene();
        edScene->OpenTab(edScene->GetAnimatorSMEditor());
    });

    UILayoutElement *buttonLE =
        p_openInEditorButton->GetGameObject()->AddComponent<UILayoutElement>();
    buttonLE->SetFlexibleSize(Vector2::One());
    buttonLE->SetLayoutPriority(3);

    AddWidget(p_openInEditorButton->GetGameObject());
}

void AIWAnimatorStateMachine::UpdateInputsFromAsset()
{
    // Empty
}

void AIWAnimatorStateMachine::OnValueChangedAIWAsset(
    EventEmitter<IEventsValueChanged> *object)
{
    BANG_UNUSED(object);
}

Texture2D *AIWAnimatorStateMachine::GetIconTexture() const
{
    return EditorTextureFactory::GetAnimatorSMIcon();
}

AnimatorStateMachine *AIWAnimatorStateMachine::GetAnimatorStateMachine() const
{
    return SCAST<AnimatorStateMachine *>(GetAsset().Get());
}
