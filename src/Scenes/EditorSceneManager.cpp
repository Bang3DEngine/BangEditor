#include "BangEditor/EditorSceneManager.h"

#include "Bang/Time.h"
#include "Bang/Scene.h"
#include "Bang/AudioManager.h"
#include "Bang/UILayoutManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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

Path EditorSceneManager::GetOpenScenePath()
{
    return SceneManager::GetActiveSceneFilepath();
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
    return p_editorScene;
}

void EditorSceneManager::_LoadSceneInstantly(Scene *scene)
{
    EditorScene *edScene = _GetEditorScene();
    if (edScene)
    {
        if (edScene->GetOpenScene() == scene) { return; }
        edScene->SetOpenScene(scene);
    }
    else // Load editor scene
    {
        p_editorScene = SCAST<EditorScene*>(scene);
        SceneManager::_LoadSceneInstantly(scene);
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
