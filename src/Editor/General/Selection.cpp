#include "BangEditor/Selection.h"

#include "Bang/GL.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneEditContainer.h"

namespace Bang
{
class GameObject;
}

using namespace Bang;
using namespace BangEditor;

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
    if(EditorCamera *edCam = EditorCamera::GetInstance())
    {
        if(SelectionFramebuffer *sfb = edCam->GetSelectionFramebuffer())
        {
            return sfb->GetGameObjectInViewportPoint(vpPoint);
        }
    }
    return nullptr;
}
