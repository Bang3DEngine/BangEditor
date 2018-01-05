#include "BangEditor/ScenePlayer.h"

#include "Bang/Scene.h"

#include "BangEditor/Editor.h"
#include "BangEditor/Behaviour.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/BehaviourContainer.h"
#include "BangEditor/EditorSceneManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ScenePlayer::ScenePlayer()
{
}

ScenePlayer::~ScenePlayer()
{
    if (p_playingScene) { GameObject::Destroy(p_playingScene); }
}

void ScenePlayer::PlayScene()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::Playing)
    {
        Editor::SetEditorPlayState(EditorPlayState::Playing);

        ScenePlayer *sp = ScenePlayer::GetInstance();

        sp->p_editingScene = EditorSceneManager::GetOpenScene();
        if (sp->p_editingScene)
        {
            sp->p_playingScene = sp->p_editingScene->Clone();

            EditorScene *edScene = EditorSceneManager::GetEditorScene();
            edScene->SetOpenScene(sp->p_playingScene, false);

            sp->InstantiatePlayingSceneBehaviours();
        }
    }
}

void ScenePlayer::StopScene()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::Editing)
    {
        Editor::SetEditorPlayState(EditorPlayState::Editing);

        ScenePlayer *sp = ScenePlayer::GetInstance();
        if (sp->p_editingScene)
        {
            EditorScene *edScene = EditorSceneManager::GetEditorScene();
            edScene->SetOpenScene(sp->p_editingScene, true);

            sp->p_editingScene = sp->p_playingScene = nullptr;
        }
    }
}

void ScenePlayer::InstantiatePlayingSceneBehaviours()
{
    ASSERT(p_playingScene);

    List<BehaviourContainer*> sceneBehaviourContainers =
            p_playingScene->GetComponentsInChildren<BehaviourContainer>(true);
    for (BehaviourContainer* behaviourContainer : sceneBehaviourContainers)
    {
        if (behaviourContainer->GetSourceFilepath().IsFile())
        {
            GameObject *go = behaviourContainer->GetGameObject();
            Behaviour *behaviour = behaviourContainer->CreateBehaviourInstance();
            go->AddComponent(behaviour);
            go->RemoveComponent(behaviourContainer);
        }
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
