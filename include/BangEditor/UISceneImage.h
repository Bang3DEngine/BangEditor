#ifndef UISCENEIMAGE_H
#define UISCENEIMAGE_H

#include "Bang/GL.h"
#include "Bang/UIImageRenderer.h"

#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneDebugStats;

class UISceneImage : public GameObject
{
    GAMEOBJECT_EDITOR(UISceneImage);

public:
    enum class RenderMode { Color = 0, Normal, Albedo, Depth,
                            Roughness, Metalness, ReceivesLight,
                            ReceivesShadows, Selection };

    UISceneImage();
    virtual ~UISceneImage();

    void Update() override;

    void SetSceneImageCamera(Camera *cam);
    void SetRenderMode(RenderMode renderMode);
    void SetShowDebugStats(bool showDebugStats);

    RenderMode GetRenderMode() const;

private:
    class UISceneImageRenderer : public UIImageRenderer
    { public:  void OnRender() override; };

    Camera *p_currentCamera = nullptr;
    RenderMode m_renderMode = RenderMode::Color;

    UISceneImageRenderer *p_sceneImg = nullptr;
    UISceneDebugStats *p_sceneDebugStats = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEIMAGE_H
