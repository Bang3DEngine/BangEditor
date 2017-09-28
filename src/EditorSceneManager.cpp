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

Scene *EditorSceneManager::GetOpenScene() const
{
    EditorScene *edScene = GetEditorScene();
    return edScene ? edScene->GetOpenScene() : nullptr;
}

EditorScene *EditorSceneManager::GetEditorScene() const
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
    Scene *_edScene = GetEditorScene();
    if (!_edScene) { return nullptr; }
    EditorScene *edScene =  SCAST<EditorScene*>(_edScene);
    return edScene->GetOpenScene();
}
