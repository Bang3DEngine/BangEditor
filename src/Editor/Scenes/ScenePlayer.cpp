#include "BangEditor/ScenePlayer.h"

#include "Bang/Scene.h"
#include "Bang/Physics.h"
#include "Bang/Behaviour.h"
#include "Bang/TimeSingleton.h"
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
    ShortcutManager::RegisterShortcut(Shortcut(Key::P, KeyModifier::LCTRL, "Play"),
                                      &ScenePlayer::OnShortcutPressed);
    ShortcutManager::RegisterShortcut(Shortcut(Key::P, (KeyModifier::LSHIFT |
                                                        KeyModifier::LCTRL),
                                               "Pause", true),
                                      &ScenePlayer::OnShortcutPressed);
}

ScenePlayer::~ScenePlayer()
{
}

void ScenePlayer::Update()
{
    if (m_steppingFrame)
    {
        ScenePlayer::PauseScene();
        m_steppingFrame = false;
    }
}

void ScenePlayer::OnShortcutPressed(const Shortcut &shortcut)
{
    if (shortcut.GetName() == "Play")
    {
        if (!Editor::IsEditingScene())
        {
            StopScene();
        }
        else
        {
            PlayScene();
        }
    }
    else if (shortcut.GetName() == "Pause")
    {
        if (GetPlayState() == PlayState::PLAYING)
        {
            PauseScene();
        }
        else if (GetPlayState() == PlayState::PAUSED)
        {
            PlayScene();
        }
    }
}

void ScenePlayer::SetPlayState(PlayState playState)
{
    if (playState != ScenePlayer::GetPlayState())
    {
        PlayState previousPlayState = ScenePlayer::GetPlayState();

        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_currentPlayState = playState;

        sp->EventEmitter<IEventsScenePlayer>::PropagateToListeners(
                    &IEventsScenePlayer::OnPlayStateChanged,
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
    return ScenePlayer::GetPlayState() == PlayState::EDITING;
}

void ScenePlayer::PlayScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::PLAYING)
    {
        ScenePlayer *sp = ScenePlayer::GetInstance();
        if (ScenePlayer::GetPlayState() == PlayState::EDITING)
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
                    ScenePlayer::SetPlayState(PlayState::JUST_BEFORE_PLAYING);

                    // Create new scene cloning the open scene into it
                    sp->p_playOpenScene = GameObjectFactory::CreateScene(false);
                    openScene->CloneInto(sp->p_playOpenScene);

                    // Close the open scene
                    SceneManager::LoadSceneInstantly(nullptr, false);

                    // Now set the open scene in the editor
                    SceneManager::LoadSceneInstantly(sp->p_playOpenScene, false);

                    TimeSingleton::SetDeltaTimeReferenceToNow();
                    Physics::GetInstance()->SetIgnoreNextFrames(sp->p_playOpenScene, 5);
                    ScenePlayer::SetPlayState(PlayState::PLAYING);
                }
            }
        }
        else if (ScenePlayer::GetPlayState() == PlayState::PAUSED)
        {
            AudioManager::ResumeAllSounds();
            ScenePlayer::SetPlayState(PlayState::PLAYING);
        }
    }
}

void ScenePlayer::PauseScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::PAUSED)
    {
        ScenePlayer::SetPlayState(PlayState::PAUSED);
        AudioManager::PauseAllSounds();
    }
}

void ScenePlayer::StepFrame()
{
    ScenePlayer::PlayScene();
    ScenePlayer *sp = ScenePlayer::GetInstance();
    sp->m_steppingFrame = true;
}

void ScenePlayer::StopScene()
{
    if (ScenePlayer::GetPlayState() != PlayState::EDITING)
    {
        ScenePlayer *sp = ScenePlayer::GetInstance();
        SceneManager::LoadSceneInstantly(sp->p_editOpenScene, false);
        if (sp->p_playOpenScene)
        {
            GameObject::Destroy(sp->p_playOpenScene);
        }

        ScenePlayer::SetPlayState(PlayState::EDITING);
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
