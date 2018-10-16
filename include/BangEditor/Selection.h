#ifndef SELECTION_H
#define SELECTION_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Camera;
FORWARD class GameObject;
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class SelectionFramebuffer;

class Selection
{
public:
    static bool IsBeingRendered();
    static Bang::GameObject *GetOveredGameObject();
    static SelectionFramebuffer *GetSelectionFramebuffer();
    static Bang::GameObject *GetOveredGameObject(const Bang::Vector2i &vpPoint);
    Selection() = delete;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTION_H
