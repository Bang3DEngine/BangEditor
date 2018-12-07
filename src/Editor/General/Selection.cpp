#include "BangEditor/Selection.h"

#include "Bang/Camera.h"
#include "Bang/Debug.h"
#include "Bang/GL.h"
#include "Bang/Geometry.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Ray.h"
#include "Bang/Scene.h"
#include "Bang/Transform.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneEditContainer.h"

namespace Bang
{
class GameObject;
}

using namespace Bang;
using namespace BangEditor;

void Selection::RegisterExtraGameObjectForNextFrame(GameObject *go)
{
    if (Selection *sel = Selection::GetInstance())
    {
        sel->p_extraGameObjects.PushBack(go);
    }
}

bool Selection::IsBeingRendered()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    SelectionFramebuffer *sfb =
        (edCam ? edCam->GetSelectionFramebuffer() : nullptr);
    return sfb ? GL::IsBound(sfb) : false;
}

GameObject *Selection::GetOveredGameObject()
{
    const Vector2i &vpPoint =
        UISceneEditContainer::GetMousePositionInOpenScene();
    return GetOveredGameObject(vpPoint);
}

SelectionFramebuffer *Selection::GetSelectionFramebuffer()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return edCam ? edCam->GetSelectionFramebuffer() : nullptr;
}

GameObject *Selection::GetOveredGameObject(const Vector2i &vpPoint)
{
    Selection *sel = Selection::GetInstance();
    if (!sel)
    {
        return nullptr;
    }

    GameObject *intersectedGo = nullptr;
    {
        // intersectedGo = GetOveredGameObject(vpPoint,
        // sel->p_extraGameObjects);
        // if (!intersectedGo)
        {
            if (Scene *openScene = EditorSceneManager::GetOpenScene())
            {
                intersectedGo = GetOveredGameObject(vpPoint, {openScene});
            }
        }
    }

    return intersectedGo;
}

GameObject *Selection::GetOveredGameObject(
    const Vector2i &vpPoint,
    const Array<GameObject *> &gameObjects)
{
    Selection *sel = Selection::GetInstance();
    if (!sel)
    {
        return nullptr;
    }

    GameObject *intersectedGo = nullptr;
    if (EditorCamera *edCam = EditorCamera::GetInstance())
    {
        Camera *cam = edCam->GetCamera();
        const Vector2 mouseNDC =
            (Vector2(vpPoint) / Vector2(cam->GetRenderSize())) * 2.0f - 1.0f;
        const Ray camRay = cam->FromViewportPointNDCToRay(mouseNDC);

        float minIntersectionDist = Math::Infinity<float>();
        for (GameObject *go : gameObjects)
        {
            if (!go)  // || !go->IsEnabledRecursively())
            {
                continue;
            }

            Array<MeshRenderer *> mrs =
                go->GetComponentsInDescendantsAndThis<MeshRenderer>();
            for (MeshRenderer *mr : mrs)
            {
                if (!mr->GetGameObject()->IsEnabledRecursively())
                {
                    continue;
                }

                bool intersected = false;
                float minDistForRend = Math::Infinity<float>();
                auto it = sel->p_cache.Find(std::make_pair(vpPoint, mr));
                if (false && it != sel->p_cache.End())
                {
                    minDistForRend = it->second;
                    intersected = (minDistForRend != Math::Infinity<float>());
                }
                else
                {
                    if (Mesh *mesh = mr->GetActiveMesh())
                    {
                        float dist = Math::Infinity<float>();
                        Geometry::IntersectRayAABox(
                            camRay,
                            mr->GetGameObject()->GetAABBoxWorld(),
                            &intersected,
                            &dist);
                        if (intersected)
                        {
                            const Matrix4 localToWorld =
                                mr->GetGameObject()
                                    ->GetTransform()
                                    ->GetLocalToWorldMatrix();
                            for (Mesh::TriangleId triId = 0;
                                 triId < mesh->GetNumTriangles();
                                 ++triId)
                            {
                                Triangle tri = mesh->GetTriangle(triId);
                                tri = localToWorld * tri;

                                Geometry::IntersectRayTriangle(
                                    camRay, tri, &intersected, &dist);

                                if (intersected && dist < minDistForRend)
                                {
                                    minDistForRend = dist;
                                    intersectedGo = mr->GetGameObject();
                                }
                            }
                        }
                    }
                }

                if (intersected && minDistForRend < minIntersectionDist)
                {
                    minIntersectionDist = minDistForRend;
                    intersectedGo = mr->GetGameObject();
                }

                sel->p_cache.Add(std::make_pair(vpPoint, mr), minDistForRend);
            }
        }
    }
    return intersectedGo;
}

void Selection::OnNewFrame()
{
    p_extraGameObjects.Clear();
    p_cache.Clear();
}

Selection *Selection::GetInstance()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return edCam ? edCam->GetSelection() : nullptr;
}
