#include "BangEditor/ScenePlayer.h"

#include "Bang/Scene.h"
#include "Bang/Behaviour.h"
#include "Bang/BehaviourContainer.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorBehaviourManager.h"

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
            Time::EstablishDeltaTimeReferenceToNow();
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

    EditorBehaviourManager *behaviourMgr = EditorBehaviourManager::GetInstance();

    behaviourMgr->PrepareBehavioursLibrary();
    Library *behavioursLib = behaviourMgr->GetBehavioursLibrary();
    if (behavioursLib)
    {
        List<BehaviourContainer*> sceneBehaviourContainers =
                p_playingScene->GetComponentsInChildren<BehaviourContainer>(true);
        for (BehaviourContainer* behaviourContainer : sceneBehaviourContainers)
        {
            behaviourContainer->SubstituteByBehaviourInstance(behavioursLib);
        }
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
