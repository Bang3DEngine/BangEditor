#ifndef SELECTION_H
#define SELECTION_H

#include "Bang/Array.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Map.h"
#include "Bang/UMap.h"
#include "Bang/Vector2.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Ray;
class Camera;
class Mesh;
class Material;
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

private:
    bool m_selectionGosGatheredForThisFrame = false;
    Bang::Map<Bang::Vector2i, Bang::GameObject *> p_cache;
    Bang::Array<Bang::GameObject *> p_extraGameObjects;

    Selection() = default;

    static bool IsFilteredByTexture(Bang::MeshRenderer *mr,
                                    const Bang::Vector3 &point,
                                    uint triId);
    static Bang::GameObject *GetOveredGameObject(
        const Bang::Vector2i &vpPoint,
        const Bang::Array<Bang::GameObject *> &gameObjects);
    static Bang::Ray GetCamRay(const Bang::Vector2i &vpPoint);

    void OnNewFrame();

    static Selection *GetInstance();

    friend class EditorCamera;
};
}

#endif  // SELECTION_H
