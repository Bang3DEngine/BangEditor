#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/GL.h"
#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class SceneDebugStats;

class UISceneContainer : public GameObject,
                         public IEditorListener
{
    GAMEOBJECT_EDITOR(UISceneContainer);

public:
    UISceneContainer();
    virtual ~UISceneContainer();

    void Update() override;

    void SetShowDebugStats(bool showDebugStats);
    void SetSceneImageTexture(Texture2D *texture);

    Rect GetImageScreenRectNDC() const;

private:
    Camera *p_selectedCamera = nullptr;

    UIImageRenderer *p_sceneImg = nullptr;
    UIImageRenderer *p_cameraPreviewImg = nullptr;

    SceneDebugStats *p_sceneDebugStats = nullptr;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SUBSCENE_H
