#ifndef TRANSFORMGIZMO_H
#define TRANSFORMGIZMO_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/MetaNode.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class GameObject;
class Scene;
}

using namespace Bang;
namespace BangEditor
{
class RectTransformSelectionGizmo;
class RotateGizmo;
class ScaleGizmo;
class TranslateGizmo;

enum class TransformGizmoCoordSpace
{
    LOCAL,
    GLOBAL
};

enum class TransformGizmoMode
{
    NONE,
    TRANSLATE,
    ROTATE,
    SCALE,
    RECT
};

class TransformGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TransformGizmo);

public:
    TransformGizmo();

    // GameObject
    virtual void Update() override;
    virtual void Render(RenderPass rp, bool renderChildren) override;

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    // SelectionGizmo
    void OnGrabBegin() override;
    void OnGrabEnd() override;
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    float GetScaleFactor() const;

    static TransformGizmo *GetInstance();

private:
    MetaNode m_transformUndoMetaBefore;

    GameObject *p_worldGizmoContainer = nullptr;
    GameObject *p_canvasGizmoContainer = nullptr;

    TranslateGizmo *p_translateGizmo = nullptr;
    RotateGizmo *p_rotateGizmo = nullptr;
    ScaleGizmo *p_scaleGizmo = nullptr;
    RectTransformSelectionGizmo *p_rectTransformGizmo = nullptr;

    virtual ~TransformGizmo() override;
};
}

#endif  // TRANSFORMGIZMO_H
