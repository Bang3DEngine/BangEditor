#include "BangEditor/UIScenePlayContainer.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"

#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UIScenePlayContainer::UIScenePlayContainer()
{
    Editor::GetInstance()->
            EventEmitter<IEditorListener>::RegisterListener(this);
    SceneManager::GetActive()->
            EventEmitter<ISceneManagerListener>::RegisterListener(this);
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

void UIScenePlayContainer::OnPlayStateChanged(EditorPlayState, EditorPlayState)
{
}

void UIScenePlayContainer::OnSceneLoaded(Scene*, const Path&)
{
}

bool UIScenePlayContainer::NeedsToRenderScene(Scene *scene)
{
    return IsVisible();
}
