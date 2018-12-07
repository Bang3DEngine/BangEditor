#include "BangEditor/Selection.h"

#include "Bang/Camera.h"
#include "Bang/Debug.h"
#include "Bang/GL.h"
#include "Bang/Geometry.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Plane.h"
#include "Bang/Ray.h"
#include "Bang/Scene.h"
#include "Bang/Transform.h"
#include "Bang/Triangle.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/GizmosManager.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/SelectionProxy.h"
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
    GameObject *intersectedGo = nullptr;
    if (Selection *sel = Selection::GetInstance())
    {
        auto it = sel->p_cache.Find(vpPoint);
        if (it != sel->p_cache.End())
        {
            intersectedGo = it->second;
        }
        else
        {
            if (!sel->m_selectionGosGatheredForThisFrame)
            {
                GizmosManager *gm = GizmosManager::GetInstance();
                sel->p_extraGameObjects.PushBack(
                    gm->GetGameObjectsForSelection());
                sel->m_selectionGosGatheredForThisFrame = true;
            }

            intersectedGo =
                GetOveredGameObject(vpPoint, sel->p_extraGameObjects);
            if (!intersectedGo)
            {
                if (Scene *openScene = EditorSceneManager::GetOpenScene())
                {
                    intersectedGo = GetOveredGameObject(vpPoint, {openScene});
                }
            }
            sel->p_cache.Add(vpPoint, intersectedGo);
        }
    }

    if (intersectedGo)
    {
        if (SelectionProxy *selectionProxy =
                intersectedGo->GetComponent<SelectionProxy>())
        {
            intersectedGo = selectionProxy->GetTargetGameObject();
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
            if (!go)
            {
                continue;
            }

            Array<MeshRenderer *> mrs =
                go->GetComponentsInDescendantsAndThis<MeshRenderer>();
            for (MeshRenderer *mr : mrs)
            {
                if (!mr->GetGameObject()->IsEnabledRecursively() ||
                    mr->GetGameObject()->HasComponent<NotSelectableInEditor>())
                {
                    continue;
                }

                bool intersectedATri = false;
                if (Mesh *mesh = mr->GetActiveMesh())
                {
                    Transform *mrTR = mr->GetGameObject()->GetTransform();

                    bool intersected = false;
                    float dist = Math::Infinity<float>();
                    Geometry::IntersectRayAABox(
                        camRay,
                        mr->GetGameObject()->GetAABBoxWorld(),
                        &intersected,
                        &dist);
                    if (intersected && dist < minIntersectionDist)
                    {
                        float minLocalMRDist = Math::Infinity<float>();
                        const Matrix4 &localToWorldInv =
                            mrTR->GetLocalToWorldMatrixInv();
                        const Ray localRay = localToWorldInv * camRay;

                        for (Mesh::TriangleId triId = 0;
                             triId < mesh->GetNumTriangles();
                             ++triId)
                        {
                            const Triangle tri = mesh->GetTriangle(triId);

                            Geometry::IntersectRayPlane(
                                localRay, tri.GetPlane(), &intersected, &dist);
                            if (intersected && dist < minLocalMRDist)
                            {
                                Geometry::IntersectRayTriangle(
                                    localRay, tri, &intersected, &dist);

                                if (intersected && dist < minLocalMRDist)
                                {
                                    minLocalMRDist = dist;
                                    intersectedATri = true;
                                }
                            }
                        }

                        if (intersectedATri)
                        {
                            const Matrix4 &localToWorld =
                                mrTR->GetLocalToWorldMatrix();
                            const Vector3 minLocalMRPoint =
                                localRay.GetPoint(minLocalMRDist);
                            const Vector3 minWorldMRPoint =
                                localToWorld.TransformedPoint(minLocalMRPoint);
                            const float minWorldMRDist = Vector3::Distance(
                                minWorldMRPoint, camRay.GetOrigin());

                            if (minWorldMRDist < minIntersectionDist)
                            {
                                minIntersectionDist = minWorldMRDist;
                                intersectedGo = mr->GetGameObject();
                            }
                        }
                    }
                }
            }
        }
    }

    return intersectedGo;
}

void Selection::OnNewFrame()
{
    m_selectionGosGatheredForThisFrame = false;
    p_extraGameObjects.Clear();
    p_cache.Clear();
}

Selection *Selection::GetInstance()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return edCam ? edCam->GetSelection() : nullptr;
}
