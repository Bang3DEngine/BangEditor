#ifndef SELECTION_H
#define SELECTION_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Camera;
class GameObject;
class Scene;
}

namespace BangEditor
{
class SelectionFramebuffer;

class Selection
{
public:
    static bool IsBeingRendered();
    static Bang::GameObject *GetOveredGameObject();
    static SelectionFramebuffer *GetSelectionFramebuffer();
    static Bang::GameObject *GetOveredGameObject(const Bang::Vector2i &vpPoint);
    Selection() = delete;
};
}

#endif  // SELECTION_H
