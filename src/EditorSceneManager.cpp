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

EditorSceneManager::~EditorSceneManager()
{
    // Dont delete active scene because ~SceneManager does it
    if (GetOpenScene() && GetOpenScene() != GetActiveScene())
    {
        GameObject::Destroy( GetOpenScene() );
    }

    if (GetEditorScene() && GetEditorScene() != GetActiveScene())
    {
        GameObject::Destroy( GetEditorScene() );
    }
}

Scene *EditorSceneManager::GetOpenScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetInstance();
    return esm ? esm->_GetOpenScene() : nullptr;
}

EditorScene *EditorSceneManager::GetEditorScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetInstance();
    return esm ? esm->_GetEditorScene() : nullptr;
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
    SceneManager *sm = SceneManager::GetInstance();
    return sm ? Cast<EditorSceneManager*>(sm) : nullptr;
}
