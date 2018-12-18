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
#include "BangEditor/SelectionOptions.h"
#include "BangEditor/UISceneEditContainer.h"

namespace Bang
{
class GameObject;
}

using namespace Bang;
using namespace BangEditor;

GameObject *Selection::GetOveredGameObject()
{
    const Vector2i &vpPoint =
        UISceneEditContainer::GetMousePositionInOpenScene();
    return GetOveredGameObject(vpPoint);
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
                uint priority = 0;
                while (true)
                {
                    Array<GameObject *> gizmosGos =
                        gm->GetGameObjectsForSelection(priority);
                    if (gizmosGos.Size() >= 1)
                    {
                        intersectedGo = GetOveredGameObject(vpPoint, gizmosGos);
                        if (intersectedGo)
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (Scene *openScene =
                                EditorSceneManager::GetOpenScene())
                        {
                            intersectedGo =
                                GetOveredGameObject(vpPoint, {openScene});
                        }
                        break;
                    }
                    ++priority;
                }
                sel->m_selectionGosGatheredForThisFrame = true;
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

                SelectionOptions *selectionOptions =
                    mr->GetGameObject()->GetComponent<SelectionOptions>();
                Texture2D *filterTex = nullptr;
                // selectionOptions ? selectionOptions->GetFilterTexture()
                //                  : nullptr;

                bool intersected;
                Vector3 intersectionPoint;
                mr->IntersectRay(camRay,
                                 filterTex,
                                 &intersected,
                                 &intersectionPoint,
                                 nullptr);

                float dist =
                    Vector3::Distance(intersectionPoint, camRay.GetOrigin());
                if (intersected && dist < minIntersectionDist)
                {
                    minIntersectionDist = dist;
                    if (selectionOptions)
                    {
                        intersectedGo = selectionOptions->GetTargetGameObject();
                    }
                    else
                    {
                        intersectedGo = mr->GetGameObject();
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
