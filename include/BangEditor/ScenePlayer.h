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
#include "BangMath/Quaternion.h"
#include "BangMath/Vector3.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ShortcutManager.h"

namespace Bang
{
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class IEventsScenePlayer;
class Shortcut;

class ScenePlayer : public EventEmitter<IEventsScenePlayer>
{
public:
    static void PlayScene();
    static void PauseScene();
    static void StepFrame();
    static void StopScene();

    static void SetPlayState(PlayState playState);
    static PlayState GetPlayState();

    Scene *GetEditOpenScene() const;
    Scene *GetPlayOpenScene() const;
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
}

#endif  // SCENEPLAYER_H
