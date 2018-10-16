#include "BangEditor/Selection.h"

#include "Bang/GL.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneEditContainer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class GameObject;
FORWARD NAMESPACE_BANG_END

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
