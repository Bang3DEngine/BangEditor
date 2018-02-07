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
    // GameObject
    void Update() override;

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    float GetScaleFactor() const;

private:
    enum class TransformMode { Translate, Rotate, Scale, Rect };
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

