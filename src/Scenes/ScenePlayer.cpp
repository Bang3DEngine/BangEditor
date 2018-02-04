#include "BangEditor/ScenePlayer.h"

#include "Bang/Scene.h"
#include "Bang/Behaviour.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/BehaviourContainer.h"

#include "BangEditor/Editor.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/SceneOpenerSaver.h"
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
    if (p_playOpenScene) { GameObject::Destroy(p_playOpenScene); }
}

void ScenePlayer::Update()
{
    if (m_pauseInNextFrame)
    {
        ScenePlayer::PauseScene();
        m_pauseInNextFrame = false;
    }
}

void ScenePlayer::OnShortcutPressed(const Shortcut &shortcut)
{
    if (shortcut.GetName() == "Play")
    {
        if (!Editor::IsEditingScene()) { StopScene(); }
        else { PlayScene(); }
    }
}

void ScenePlayer::SetPlayState(PlayState playState)
{
    if (playState != ScenePlayer::GetPlayState())
    {
        PlayState previousPlayState = ScenePlayer::GetPlayState();

        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_currentPlayState = playState;

        sp->EventEmitter<IScenePlayerListener>::PropagateToListeners(
                    &IScenePlayerListener::OnPlayStateChanged,
                    previousPlayState,
                    sp->m_currentPlayState);

        AudioManager::SetPlayOnStartBlocked( Editor::IsEditingScene() );
    }
}

PlayState ScenePlayer::GetPlayState()
{
    ScenePlayer *sp = ScenePlayer::GetInstance();
    return sp ? sp->m_currentPlayState : Undef<PlayState>();
}

bool Editor::IsEditingScene()
{
    return ScenePlayer::GetPlayState() == PlayState::Editing;
}

void ScenePlayer::PlayScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::Playing)
    {
        ScenePlayer *sp = ScenePlayer::GetInstance();
        if (ScenePlayer::GetPlayState() == PlayState::Editing)
        {
            // Play scene!
            EditorScene *edScene = EditorSceneManager::GetEditorScene();
            EditorBehaviourManager *edBehaviourMgr = EditorBehaviourManager::GetActive();
            bool behavioursReady = edBehaviourMgr->PrepareBehavioursLibrary();
            if (behavioursReady)
            {
                ScenePlayer::SetPlayState(PlayState::Playing);

                Scene *openScene = EditorSceneManager::GetOpenScene();
                if (openScene)
                {
                    // Create empty scene, set it active, start it empty
                    sp->p_playOpenScene = GameObjectFactory::CreateScene(false);

                    EditorSceneManager::SetActiveScene(sp->p_playOpenScene);

                    // Set open scene to null first
                    edScene->SetOpenScene(nullptr, false);

                    // Clone the editing scene into the playing scene
                    openScene->CloneInto(sp->p_playOpenScene);

                    // Now set the open scene in the editor
                    edScene->SetOpenScene(sp->p_playOpenScene, false);


                    Time::SetDeltaTimeReferenceToNow();
                }
            }
        }
        sp->m_pauseInNextFrame = false;
    }
}

void ScenePlayer::PauseScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::Paused)
    {
        ScenePlayer::SetPlayState(PlayState::Paused);
        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_pauseInNextFrame = false;
    }
}

void ScenePlayer::StepFrame()
{
    if (ScenePlayer::GetPlayState() != PlayState::StepFrame)
    {
        ScenePlayer::SetPlayState(PlayState::StepFrame);
        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_pauseInNextFrame = true;
    }
}

void ScenePlayer::StopScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::Editing)
    {
        ScenePlayer::SetPlayState(PlayState::Editing);

        Path openScenePath = SceneOpenerSaver::GetInstance()->GetOpenScenePath();
        if (openScenePath.IsFile())
        {
            SceneManager::LoadSceneInstantly(openScenePath);
        }
        else { SceneManager::LoadSceneInstantly(nullptr); }

        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->p_playOpenScene = nullptr;
        sp->m_pauseInNextFrame = false;
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
