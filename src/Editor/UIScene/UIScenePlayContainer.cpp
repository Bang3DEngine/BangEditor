#include "BangEditor/UIScenePlayContainer.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Scene.h"
#include "Bang/SceneManager.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneToolbar.h"
#include "BangEditor/UISceneToolbarDown.h"

namespace Bang {
class Camera;
class Path;
}  // namespace Bang

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIScenePlayContainer::UIScenePlayContainer()
{
    GetSceneToolbar()->DisableTransformAndCameraControls();
    GetSceneToolbarDown()->SetEnabled(false);

    ScenePlayer::GetInstance()->
            EventEmitter<IEventsScenePlayer>::RegisterListener(this);
    SceneManager::GetActive()->
            EventEmitter<IEventsSceneManager>::RegisterListener(this);
}

UIScenePlayContainer::~UIScenePlayContainer()
{
}

void UIScenePlayContainer::Update()
{
    GameObject::Update();
    SetScene( EditorSceneManager::GetOpenScene() );
}

Camera* UIScenePlayContainer::GetSceneCamera(Scene *scene)
{
    return scene ? scene->GetCamera() : nullptr;
}

void UIScenePlayContainer::OnPlayStateChanged(PlayState, PlayState)
{
}

void UIScenePlayContainer::OnSceneLoaded(Scene*, const Path&)
{
}

bool UIScenePlayContainer::NeedsToRenderContainedScene(Scene *scene)
{
    return IsVisibleRecursively();
}
