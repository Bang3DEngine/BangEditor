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
            sp->p_editOpenScene = EditorSceneManager::GetOpenScene();

            // Play scene!
            EditorBehaviourManager *edBehaviourMgr = EditorBehaviourManager::GetActive();
            bool behavioursReady = edBehaviourMgr->PrepareBehavioursLibrary();
            if (behavioursReady)
            {
                Scene *openScene = EditorSceneManager::GetOpenScene();
                if (openScene)
                {
                    ScenePlayer::SetPlayState(PlayState::JustBeforePlaying);

                    // Create new scene cloning the open scene into it
                    Scene *playScene = GameObjectFactory::CreateScene(false);
                    openScene->CloneInto(playScene);

                    // Close the open scene
                    SceneManager::LoadSceneInstantly(nullptr);

                    // Now set the open scene in the editor
                    SceneManager::LoadSceneInstantly(playScene);

                    Time::SetDeltaTimeReferenceToNow();

                    ScenePlayer::SetPlayState(PlayState::Playing);
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
        ScenePlayer *sp = ScenePlayer::GetInstance();
        SceneManager::LoadSceneInstantly(sp->p_editOpenScene);

        sp->m_pauseInNextFrame = false;

        ScenePlayer::SetPlayState(PlayState::Editing);
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
