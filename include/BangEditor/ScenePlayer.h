#ifndef SCENEPLAYER_H
#define SCENEPLAYER_H

#include "Bang/Bang.h"
#include "Bang/Path.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ShortcutManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

enum class PlayState { JUST_BEFORE_PLAYING, PLAYING, PAUSED, EDITING };

class IScenePlayerListener : public virtual IEventListener
{
    EVENTLISTENER(IScenePlayerListener)

public:
    virtual void OnPlayStateChanged(PlayState previousPlayState,
                                    PlayState newPlayState)
    { (void)(previousPlayState); (void)(newPlayState); }
};

class ScenePlayer : public EventEmitter<IScenePlayerListener>
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
    PlayState m_currentPlayState;

    bool m_steppingFrame = false;
    Scene *p_editOpenScene = nullptr;
    Scene *p_playOpenScene = nullptr;

    ScenePlayer();
    virtual ~ScenePlayer();

    void Update();

    static void OnShortcutPressed(const Shortcut &shortcut);

    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEPLAYER_H

