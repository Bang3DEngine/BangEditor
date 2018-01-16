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
    ShortcutManager::RegisterShortcut(Shortcut(Key::LCtrl, Key::P, "Play"),
                                      &ScenePlayer::OnShortcutPressed);
}

ScenePlayer::~ScenePlayer()
{
    if (p_playingScene) { GameObject::Destroy(p_playingScene); }
}

void ScenePlayer::OnShortcutPressed(const Shortcut &shortcut)
{
    if (shortcut.GetName() == "Play")
    {
        if (Editor::IsPlaying()) { StopScene(); }
        else { PlayScene(); }
    }
}

void ScenePlayer::PlayScene()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::Playing)
    {
        EditorBehaviourManager *behaviourMgr = EditorBehaviourManager::GetInstance();
        bool behavioursReady = behaviourMgr->PrepareBehavioursLibrary();
        if (behavioursReady)
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

bool ScenePlayer::InstantiatePlayingSceneBehaviours()
{
    ASSERT(p_playingScene);

    EditorBehaviourManager *behaviourMgr = EditorBehaviourManager::GetInstance();

    if (!behaviourMgr->PrepareBehavioursLibrary()) { return false; }

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

    return (behavioursLib != nullptr);
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
