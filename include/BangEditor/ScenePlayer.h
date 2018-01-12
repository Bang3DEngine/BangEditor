#ifndef SCENEPLAYER_H
#define SCENEPLAYER_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ScenePlayer
{
public:
    static void PlayScene();
    static void StopScene();

private:
    Scene *p_editingScene = nullptr;
    Scene *p_playingScene = nullptr;

    bool InstantiatePlayingSceneBehaviours();

    ScenePlayer();
    virtual ~ScenePlayer();

    static ScenePlayer *GetInstance();

    friend class EditorScene;
};

NAMESPACE_BANG_EDITOR_END

#endif // SCENEPLAYER_H

