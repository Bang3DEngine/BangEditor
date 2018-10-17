#ifndef UISCENEIMAGE_H
#define UISCENEIMAGE_H

#include "Bang/BangDefines.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "Bang/UIImageRenderer.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Editor.h"

namespace Bang
{
class Camera;
class Texture2D;
}

using namespace Bang;
namespace BangEditor
{
class UISceneDebugStats;

class UISceneImage : public GameObject
{
    GAMEOBJECT_EDITOR(UISceneImage);

public:
    enum class RenderMode
    {
        COLOR = 0,
        NORMAL,
        ALBEDO,
        DEPTH,
        ROUGHNESS,
        METALNESS,
        RECEIVES_LIGHT,
        RECEIVES_SHADOWS,
        SELECTION,
        WORLD_POSITION
    };

    class UISceneImageRenderer : public UIImageRenderer
    {
    public:
        void OnRender() override;
    };

    UISceneImage();
    virtual ~UISceneImage() override;

    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    void SetSceneImageCamera(Camera *cam);
    void SetRenderMode(RenderMode renderMode);
    void SetShowDebugStats(bool showDebugStats);

    Camera *GetCamera() const;
    RenderMode GetRenderMode() const;
    UISceneImageRenderer *GetSceneImageRenderer() const;

private:
    Camera *p_currentCamera = nullptr;
    RenderMode m_renderMode = RenderMode::COLOR;

    UISceneImageRenderer *p_sceneImg = nullptr;
    UISceneDebugStats *p_sceneDebugStats = nullptr;
};
}

#endif  // UISCENEIMAGE_H
