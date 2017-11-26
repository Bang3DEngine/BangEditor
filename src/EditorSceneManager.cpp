#include "BangEditor/EditorSceneManager.h"

#include "Bang/Time.h"
#include "Bang/UILayoutManager.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void EditorSceneManager::_Update()
{
    SceneManager::UpdateScene( GetActiveScene() );
}

Scene *EditorSceneManager::GetOpenScene()
{
    return EditorSceneManager::GetInstance()->_GetOpenScene();
}

EditorScene *EditorSceneManager::GetEditorScene()
{
    return EditorSceneManager::GetInstance()->_GetEditorScene();
}

Scene *EditorSceneManager::_GetOpenScene() const
{
    EditorScene *edScene = GetEditorScene();
    return edScene ? edScene->GetOpenScene() : nullptr;
}

EditorScene *EditorSceneManager::_GetEditorScene() const
{
    Scene *edScene = _GetActiveScene();
    return edScene ? Cast<EditorScene*>(edScene) : nullptr;
}

void EditorSceneManager::_LoadScene(Scene *scene)
{
    EditorScene *edScene = GetEditorScene();
    if (edScene)
    {
        ENSURE(edScene->GetOpenScene() != scene);
        edScene->SetOpenScene(scene);
    }
    else // Load editor scene
    {
        SceneManager::_LoadScene(scene);
    }
}

void EditorSceneManager::SetActiveScene(Scene *activeScene)
{
    GetInstance()->_SetActiveScene(activeScene);
}

EditorSceneManager *EditorSceneManager::GetInstance()
{
    return Cast<EditorSceneManager*>(SceneManager::GetInstance());
}
