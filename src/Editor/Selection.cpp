#include "BangEditor/Selection.h"

#include "Bang/Scene.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneEditContainer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

bool Selection::IsBeingRendered()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    SelectionFramebuffer *sfb = (edCam ? edCam->GetSelectionFramebuffer() :
                                         nullptr);
    return sfb ? GL::IsBound(sfb) : false;
}

GameObject *Selection::GetOveredGameObject()
{
    const Vector2i &vpPoint = UISceneEditContainer::GetMousePositionInOpenScene();
    return GetOveredGameObject(vpPoint);
}

SelectionFramebuffer *Selection::GetSelectionFramebuffer()
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return edCam ? edCam->GetSelectionFramebuffer() : nullptr;
}

GameObject *Selection::GetOveredGameObject(const Vector2i &vpPoint)
{
    if (EditorCamera *edCam = EditorCamera::GetInstance())
    {
        if (SelectionFramebuffer *sfb = edCam->GetSelectionFramebuffer())
        {
            return sfb->GetGameObjectInViewportPoint(vpPoint);
        }
    }
    return nullptr;

}
