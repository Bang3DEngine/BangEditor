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
    SceneManager::GetInstance()->
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
    Camera *selectedCamera = nullptr;
    List<Camera*> cameras = scene->GetComponentsInChildren<Camera>(true);
    for (Camera *cam : cameras)
    {
        if (!EditorCamera::IsEditorCamera(cam))
        {
            selectedCamera = cam;
            break;
        }
    }
    return selectedCamera;
}

void UIScenePlayContainer::OnPlayStateChanged(EditorPlayState, EditorPlayState)
{
}

void UIScenePlayContainer::OnSceneOpen(Scene*, const Path&)
{
}
