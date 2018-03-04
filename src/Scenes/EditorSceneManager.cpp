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
    EventEmitter<ISceneManagerListener>::RegisterListener(this);
}

EditorSceneManager::~EditorSceneManager()
{
    if (GetOpenScene()) { GameObject::Destroy( GetOpenScene() ); }
    if (GetEditorScene()) { GameObject::Destroy( GetEditorScene() ); }
}

Scene *EditorSceneManager::GetOpenScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    return esm ? esm->_GetOpenScene() : nullptr;
}

EditorScene *EditorSceneManager::GetEditorScene()
{
    EditorSceneManager *esm = EditorSceneManager::GetActive();
    return esm ? esm->_GetEditorScene() : nullptr;
}

EditorBehaviourManager* EditorSceneManager::GetEditorBehaviourManager() const
{
    return DCAST<EditorBehaviourManager*>( SceneManager::GetBehaviourManager() );
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

void EditorSceneManager::SetActiveScene(Scene *activeScene)
{
    GetActive()->SetActiveScene_(activeScene);
}

BehaviourManager *EditorSceneManager::CreateBehaviourManager() const
{
    return new EditorBehaviourManager();
}

void EditorSceneManager::OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
{
    if (_GetEditorScene())
    {
        _GetEditorScene()->SetOpenScene( GetLoadedScene() );
    }
    else // Retrieve editor scene
    {
        p_editorScene = SCAST<EditorScene*>( GetLoadedScene() );
    }
}

EditorSceneManager *EditorSceneManager::GetActive()
{
    SceneManager *sm = SceneManager::GetActive();
    return sm ? Cast<EditorSceneManager*>(sm) : nullptr;
}
