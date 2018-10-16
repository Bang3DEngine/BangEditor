#ifndef SCENEPLAYER_H
#define SCENEPLAYER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.h"
#include "Bang/EventListener.tcc"
#include "Bang/Path.h"
#include "Bang/Quaternion.h"
#include "Bang/Vector3.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ShortcutManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class IEventsScenePlayer;
FORWARD class Shortcut;

class ScenePlayer : public EventEmitter<IEventsScenePlayer>
{
public:
    static void PlayScene();
    static void PauseScene();
    static void StepFrame();
    static void StopScene();

    static void SetPlayState(PlayState playState);
    static PlayState GetPlayState();

    static ScenePlayer *GetInstance();

private:
    PlayState m_currentPlayState = PlayState::EDITING;

    bool m_steppingFrame = false;
    Scene *p_editOpenScene = nullptr;
    Scene *p_playOpenScene = nullptr;

    ScenePlayer();
    virtual ~ScenePlayer() override;

    void Update();

    static void OnShortcutPressed(const Shortcut &shortcut);

    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEPLAYER_H

