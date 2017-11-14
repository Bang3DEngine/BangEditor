#include "BangEditor/EditorSceneManager.h"

#include "Bang/Time.h"
#include "Bang/UILayoutManager.h"
#include "BangEditor/EditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorSceneManager::EditorSceneManager()
{

}

EditorSceneManager::~EditorSceneManager()
{

}

void EditorSceneManager::_Update()
{
    SceneManager::UpdateScene(GetRootScene());
    SceneManager::UpdateScene(GetOpenScene());
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
    Scene *edScene = _GetRootScene();
    return edScene ? SCAST<EditorScene*>(edScene) : nullptr;
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

Scene *EditorSceneManager::_GetRootScene() const
{
    return m_activeScene;
}

Scene *EditorSceneManager::_GetActiveScene() const
{
    EditorScene *edScene = GetEditorScene();
    return edScene->GetOpenScene();
}

EditorSceneManager *EditorSceneManager::GetInstance()
{
    return DCAST<EditorSceneManager*>(SceneManager::GetInstance());
}
