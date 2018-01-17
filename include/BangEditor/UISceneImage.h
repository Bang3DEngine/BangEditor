#ifndef UISCENEIMAGE_H
#define UISCENEIMAGE_H

#include "Bang/GL.h"
#include "Bang/GameObject.h"

#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneDebugStats;

class UISceneImage : public GameObject,
                     public IEditorListener
{
    GAMEOBJECT_EDITOR(UISceneImage);

public:
    UISceneImage();
    virtual ~UISceneImage();

    void Update() override;

    void SetShowDebugStats(bool showDebugStats);
    void SetSceneImageTexture(Texture2D *texture);

    Rect GetImageScreenRectNDC() const;

private:
    Camera *p_selectedCamera = nullptr;

    UIImageRenderer *p_sceneImg = nullptr;
    UIImageRenderer *p_cameraPreviewImg = nullptr;

    UISceneDebugStats *p_sceneDebugStats = nullptr;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEIMAGE_H
