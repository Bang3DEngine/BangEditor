#ifndef TRANSFORMGIZMO_H
#define TRANSFORMGIZMO_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/MetaNode.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class GameObject;
FORWARD class Scene;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RectTransformSelectionGizmo;
FORWARD class RotateGizmo;
FORWARD class ScaleGizmo;
FORWARD class TranslateGizmo;

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
    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

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

    GameObject                  *p_worldGizmoContainer  = nullptr;
    GameObject                  *p_canvasGizmoContainer = nullptr;

    TranslateGizmo              *p_translateGizmo     = nullptr;
    RotateGizmo                 *p_rotateGizmo        = nullptr;
    ScaleGizmo                  *p_scaleGizmo         = nullptr;
    RectTransformSelectionGizmo *p_rectTransformGizmo = nullptr;

	TransformGizmo();
    virtual ~TransformGizmo() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // TRANSFORMGIZMO_H

