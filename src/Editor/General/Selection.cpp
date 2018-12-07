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
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "Bang/Triangle.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/GizmosManager.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/SelectionOptions.h"
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
        if (SelectionOptions *selectionOptions =
                intersectedGo->GetComponent<SelectionOptions>())
        {
            intersectedGo = selectionOptions->GetTargetGameObject();
        }
    }

    return intersectedGo;
}

bool Selection::IsFilteredByTexture(MeshRenderer *mr,
                                    const Vector3 &point,
                                    uint triId)
{
    SelectionOptions *selectionOptions =
        mr->GetGameObject()->GetComponent<SelectionOptions>();
    if (selectionOptions && mr->GetActiveMaterial())
    {
        Mesh *mesh = mr->GetActiveMesh();
        Texture2D *tex = selectionOptions->GetFilterTexture();
        if (!tex)
        {
            return false;
        }

        Triangle tri = mesh->GetTriangle(triId);

        uint triV0Id = mesh->GetTrianglesVertexIds()[triId * 3 + 0];
        uint triV1Id = mesh->GetTrianglesVertexIds()[triId * 3 + 1];
        uint triV2Id = mesh->GetTrianglesVertexIds()[triId * 3 + 2];
        const Array<Vector2> &uvs = mesh->GetUvsPool();
        Vector2 uvs0 = triV0Id < uvs.Size() ? uvs[triV0Id] : Vector2::Zero();
        Vector2 uvs1 = triV1Id < uvs.Size() ? uvs[triV1Id] : Vector2::Zero();
        Vector2 uvs2 = triV2Id < uvs.Size() ? uvs[triV2Id] : Vector2::Zero();

        Vector3 barycentricCoords = tri.GetBarycentricCoordinates(point);
        Vector2 pointUvs = (uvs0 * barycentricCoords[0]) +
                           (uvs1 * barycentricCoords[1]) +
                           (uvs2 * barycentricCoords[2]);
        pointUvs.y = (1.0f - pointUvs.y);
        Vector2i pixelCoords(Vector2(tex->GetImage().GetSize()) * pointUvs);

        if (tex && pixelCoords >= Vector2i::Zero() &&
            pixelCoords < tex->GetImage().GetSize())
        {
            Color pixelColor =
                tex->GetImage().GetPixel(pixelCoords.x, pixelCoords.y);
            if (pixelColor.a <= tex->GetAlphaCutoff())
            {
                return true;
            }
        }
    }
    return false;
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
                    SelectionOptions *selectionOptions =
                        mr->GetGameObject()->GetComponent<SelectionOptions>();

                    bool intersected = false;
                    float dist = Math::Infinity<float>();
                    Geometry::IntersectRayAABox(
                        camRay,
                        mr->GetGameObject()->GetAABBoxWorld(),
                        &intersected,
                        &dist);
                    if (intersected && dist < minIntersectionDist)
                    {
                        const Matrix4 &localToWorldInv =
                            mrTR->GetLocalToWorldMatrixInv();
                        const Ray localRay = localToWorldInv * camRay;

                        float minLocalMRDist = Math::Infinity<float>();
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
                                    GameObject *targetGo = intersectedGo;
                                    bool filteredByTexture = false;
                                    if (selectionOptions)
                                    {
                                        Vector3 localMRPoint =
                                            localRay.GetPoint(dist);
                                        filteredByTexture = IsFilteredByTexture(
                                            mr, localMRPoint, triId);
                                        targetGo = selectionOptions
                                                       ->GetTargetGameObject();
                                    }

                                    if (!filteredByTexture)
                                    {
                                        intersectedGo = targetGo;
                                        minLocalMRDist = dist;
                                        intersectedATri = true;
                                    }
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
