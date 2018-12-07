#ifndef SELECTION_H
#define SELECTION_H

#include "Bang/Array.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Map.h"
#include "Bang/Vector2.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Ray;
class Camera;
class GameObject;
class MeshRenderer;
class Scene;
}

namespace BangEditor
{
class SelectionFramebuffer;

class Selection
{
public:
    static void RegisterExtraGameObjectForNextFrame(Bang::GameObject *go);

    static bool IsBeingRendered();
    static Bang::GameObject *GetOveredGameObject();
    static SelectionFramebuffer *GetSelectionFramebuffer();
    static Bang::GameObject *GetOveredGameObject(const Bang::Vector2i &vpPoint);
    static Bang::GameObject *GetOveredGameObject(
        const Bang::Vector2i &vpPoint,
        const Bang::Array<Bang::GameObject *> &gameObjects);
    static bool IsOvered(Bang::GameObject *go);
    static bool IsOvered(const Bang::Vector2i &vpPoint, Bang::GameObject *go);
    static void IsOvered(const Bang::Ray &ray,
                         Bang::GameObject *go,
                         bool *outIntersected,
                         float *outDist = nullptr);

private:
    Bang::Map<std::pair<Bang::Vector2i, Bang::MeshRenderer *>, float> p_cache;
    Bang::Array<Bang::GameObject *> p_extraGameObjects;

    Selection() = default;

    static Bang::Ray GetCamRay(const Bang::Vector2i &vpPoint);
    void OnNewFrame();

    static Selection *GetInstance();

    friend class EditorCamera;
};
}

#endif  // SELECTION_H
