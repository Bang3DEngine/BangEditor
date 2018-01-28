#include "BangEditor/ScenePlayer.h"

#include "Bang/Scene.h"
#include "Bang/Behaviour.h"
#include "Bang/GameObjectFactory.h"
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

void ScenePlayer::PlayScene()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::Playing)
    {
        ScenePlayer *sp = ScenePlayer::GetInstance();
        if (Editor::GetEditorPlayState() == EditorPlayState::Editing)
        {
            Editor::SetEditorPlayState(EditorPlayState::Playing);

            // Play scene!
            EditorScene *edScene = EditorSceneManager::GetEditorScene();
            EditorBehaviourManager *edBehaviourMgr = EditorBehaviourManager::GetActive();
            bool behavioursReady = edBehaviourMgr->PrepareBehavioursLibrary();
            if (behavioursReady)
            {
                sp->m_prevOpenScenePath = SceneManager::GetActiveSceneFilepath();
                Scene *openScene = EditorSceneManager::GetOpenScene();
                if (openScene)
                {
                    // Create empty scene, set it active, start it empty
                    sp->p_playOpenScene = GameObjectFactory::CreateScene(false);
                    edScene->GetLocalObjectManager()->StartObjects();

                    EditorSceneManager::SetActiveScene(sp->p_playOpenScene);

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
    if (Editor::GetEditorPlayState() != EditorPlayState::Paused)
    {
        Editor::SetEditorPlayState(EditorPlayState::Paused);
        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_pauseInNextFrame = false;
    }
}

void ScenePlayer::StepFrame()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::StepFrame)
    {
        Editor::SetEditorPlayState(EditorPlayState::StepFrame);
        ScenePlayer *sp = ScenePlayer::GetInstance();
        sp->m_pauseInNextFrame = true;
    }
}

void ScenePlayer::StopScene()
{
    if (Editor::GetEditorPlayState() != EditorPlayState::Editing)
    {
        Editor::SetEditorPlayState(EditorPlayState::Editing);

        ScenePlayer *sp = ScenePlayer::GetInstance();
        SceneManager::LoadScene(sp->m_prevOpenScenePath);

        sp->p_playOpenScene = nullptr;
        sp->m_pauseInNextFrame = false;
    }
}

ScenePlayer *ScenePlayer::GetInstance()
{
    return EditorSceneManager::GetEditorScene()->GetScenePlayer();
}
