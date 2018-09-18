#include "BangEditor/EditorSceneManager.h"

#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/AudioManager.h"
#include "Bang/UILayoutManager.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorBehaviourManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

EditorSceneManager::EditorSceneManager()
{
    EventEmitter<IEventsSceneManager>::RegisterListener(this);
}

EditorSceneManager::~EditorSceneManager()
{
    if (GetOpenScene())
    {
        GameObject::Destroy( GetOpenScene() );
    }

    if (GetEditorScene())
    {
        GameObject::Destroy( GetEditorScene() );
    }
}

Scene *EditorSceneManager::GetOpenScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    return esm ? esm->GetOpenScene_() : nullptr;
}

EditorScene *EditorSceneManager::GetEditorScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    return esm ? esm->GetEditorScene_() : nullptr;
}

EditorBehaviourManager* EditorSceneManager::GetEditorBehaviourManager() const
{
    return DCAST<EditorBehaviourManager*>( SceneManager::GetBehaviourManager() );
}

Scene *EditorSceneManager::GetOpenScene_() const
{
    EditorScene *edScene = GetEditorScene();
    return edScene ? edScene->GetOpenScene() : nullptr;
}

EditorScene *EditorSceneManager::GetEditorScene_() const
{
    return p_editorScene;
}

void EditorSceneManager::SetActiveScene(Scene *activeScene)
{
    EditorSceneManager::GetActive()->SetActiveScene_(activeScene);
}

BehaviourManager *EditorSceneManager::CreateBehaviourManager() const
{
    return new EditorBehaviourManager();
}

void EditorSceneManager::OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
{
    BANG_UNUSED_2(scene, sceneFilepath);
    if (GetEditorScene_())
    {
        GetEditorScene_()->SetOpenScene( GetLoadedScene() );
    }
    else // Retrieve editor scene
    {
        p_editorScene = DCAST<EditorScene*>( GetLoadedScene() );
    }
}

EditorSceneManager *EditorSceneManager::GetActive()
{
    SceneManager *sm = SceneManager::GetActive();
    return sm ? DCAST<EditorSceneManager*>(sm) : nullptr;
}
