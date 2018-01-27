#ifndef SCENEPLAYER_H
#define SCENEPLAYER_H

#include "Bang/Bang.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ShortcutManager.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ScenePlayer
{
public:
    static void PlayScene();
    static void PauseScene();
    static void StepFrame();
    static void StopScene();

private:
    bool m_pauseInNextFrame = false;
    Scene *p_editOpenScene = nullptr;
    Scene *p_playOpenScene = nullptr;

    ScenePlayer();
    virtual ~ScenePlayer();

    void Update();

    static void OnShortcutPressed(const Shortcut &shortcut);

    static ScenePlayer *GetInstance();

    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEPLAYER_H

