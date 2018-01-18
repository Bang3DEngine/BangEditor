#include "BangEditor/UISceneEditContainer.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"

#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneEditContainer::UISceneEditContainer()
{
    Editor::GetInstance()->
            EventEmitter<IEditorListener>::RegisterListener(this);
    SceneManager::GetInstance()->
            EventEmitter<ISceneManagerListener>::RegisterListener(this);
}

UISceneEditContainer::~UISceneEditContainer()
{
}

void UISceneEditContainer::Update()
{
    GameObject::Update();
    SetScene( EditorSceneManager::GetOpenScene() );
}

Camera* UISceneEditContainer::GetSceneCamera(Scene *scene)
{
    Camera *editorCamera = EditorCamera::GetEditorCamera(scene);
    if (editorCamera) { return editorCamera; }
    return scene->GetCamera();
}

void UISceneEditContainer::OnPlayStateChanged(EditorPlayState, EditorPlayState)
{
}

void UISceneEditContainer::OnSceneOpen(Scene *scene, const Path&)
{
}
