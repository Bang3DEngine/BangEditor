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

EditorSceneManager::~EditorSceneManager()
{
    if (GetOpenScene() && GetOpenScene() != _GetActiveScene())
    {
        GameObject::Destroy( GetOpenScene() );
    }

    if (GetEditorScene() && GetEditorScene() != _GetActiveScene())
    {
        GameObject::Destroy( GetEditorScene() );
    }
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

Scene *EditorSceneManager::_GetActiveScene() const
{
    return GetOpenScene();
}

void EditorSceneManager::_LoadSceneInstantly()
{
    ASSERT( GetNextLoadNeeded() );

    SceneManager::PreLoadSceneInstantly();
    if (_GetEditorScene())
    {
        _GetEditorScene()->SetOpenScene( GetNextLoadScene() );
    }
    else // Retrieve editor scene
    {
        p_editorScene = SCAST<EditorScene*>( GetNextLoadScene() );
    }
    SceneManager::PostLoadSceneInstantly();
}

void EditorSceneManager::SetActiveScene(Scene *activeScene)
{
    GetActive()->_SetActiveScene(activeScene);
}

Scene *EditorSceneManager::GetSceneToBeRenderedToWindow() const
{
    return GetEditorScene();
}

void EditorSceneManager::OnResize(int width, int height)
{
    if (GetOpenScene()) { GetOpenScene()->OnResize(width, height); }
    if (GetEditorScene()) { GetEditorScene()->OnResize(width, height); }
}

void EditorSceneManager::Update()
{
    GetEditorBehaviourManager()->Update();

    if (GetNextLoadNeeded())  {  _LoadSceneInstantly(); }
    SceneManager::OnNewFrame( GetEditorScene(), true );
    SceneManager::OnNewFrame( GetOpenScene(), true );
}

void EditorSceneManager::Render()
{
    EditorScene *edScene = EditorSceneManager::GetEditorScene();
    edScene->RenderOpenScene();
    SceneManager::Render();
}

BehaviourManager *EditorSceneManager::CreateBehaviourManager() const
{
    return new EditorBehaviourManager();
}

EditorSceneManager *EditorSceneManager::GetActive()
{
    SceneManager *sm = SceneManager::GetActive();
    return sm ? Cast<EditorSceneManager*>(sm) : nullptr;
}
