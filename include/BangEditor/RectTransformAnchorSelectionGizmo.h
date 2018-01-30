#ifndef RECTTRANSFORMANCHORSELECTIONGIZMO_H
#define RECTTRANSFORMANCHORSELECTIONGIZMO_H

#include "Bang/Alignment.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RectTransformAnchorSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RectTransformAnchorSelectionGizmo);

public:
    enum class AnchorSide { LeftBot, LeftTop, RightTop, RightBot };

    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    void SetAnchorSide(AnchorSide anchorSide);

private:
    static const int AnchorSize;
    static const int AnchorSelectionSize;

    AnchorSide m_anchorSide = AnchorSide::LeftBot;
    Vector2 m_startGrabMousePosNDC = Vector2::Zero;
    Vector2 m_startAnchorMin = Vector2::Zero;
    Vector2 m_startAnchorMax = Vector2::Zero;

    GameObject *p_anchorGO = nullptr;
    GameObject *p_selectionGO = nullptr;
    UIImageRenderer *p_anchorRenderer = nullptr;
    UIImageRenderer *p_selectionRenderer = nullptr;

    RectTransformAnchorSelectionGizmo();
    virtual ~RectTransformAnchorSelectionGizmo();

    void UpdateBasedOnAnchorSide();
};

NAMESPACE_BANG_EDITOR_END

#endif // RECTTRANSFORMANCHORSELECTIONGIZMO_H

