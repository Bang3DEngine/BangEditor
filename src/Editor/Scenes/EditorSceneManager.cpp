#include "BangEditor/EditorSceneManager.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Scene.h"
#include "BangEditor/EditorBehaviourManager.h"
#include "BangEditor/EditorScene.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class BehaviourManager;
FORWARD class Path;
FORWARD NAMESPACE_BANG_END

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
