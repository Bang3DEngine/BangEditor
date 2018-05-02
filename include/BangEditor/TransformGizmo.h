#ifndef TRANSFORMGIZMO_H
#define TRANSFORMGIZMO_H

#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ScaleGizmo;
FORWARD class RotateGizmo;
FORWARD class TranslateGizmo;
FORWARD class RectTransformSelectionGizmo;

class TransformGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(TransformGizmo);

public:
    enum class TransformMode { Translate, Rotate, Scale, Rect };

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    void SetTransformMode(TransformMode transformMode);

    float GetScaleFactor() const;
    TransformMode GetTransformMode() const;

    static TransformGizmo *GetInstance();

private:
    TransformMode m_transformMode = TransformMode::Translate;

    GameObject                  *p_worldGizmoContainer  = nullptr;
    GameObject                  *p_canvasGizmoContainer = nullptr;

    TranslateGizmo              *p_translateGizmo     = nullptr;
    RotateGizmo                 *p_rotateGizmo        = nullptr;
    ScaleGizmo                  *p_scaleGizmo         = nullptr;
    RectTransformSelectionGizmo *p_rectTransformGizmo = nullptr;

	TransformGizmo();
    virtual ~TransformGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // TRANSFORMGIZMO_H

