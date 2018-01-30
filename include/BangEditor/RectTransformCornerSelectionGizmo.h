#ifndef RECTTRANSFORMCORNERSELECTIONGIZMO_H
#define RECTTRANSFORMCORNERSELECTIONGIZMO_H

#include "Bang/Alignment.h"
#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class RectTransformCornerSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RectTransformCornerSelectionGizmo);

public:
    enum class CornerSide { LeftBot, LeftTop, RightTop, RightBot, Center };

    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    void SetCornerSide(CornerSide cornerSide);

private:
    static const int CornerSize;
    static const int CornerSelectionSize;

    CornerSide m_cornerSide = CornerSide::LeftBot;
    Vector2i m_startGrabMousePos   = Vector2i::Zero;
    Vector2i m_startMarginLeftBot  = Vector2i::Zero;
    Vector2i m_startMarginRightTop = Vector2i::Zero;

    GameObject *p_cornerGO = nullptr;
    GameObject *p_selectionGO = nullptr;
    UIImageRenderer *p_cornerRenderer = nullptr;
    UIImageRenderer *p_selectionRenderer = nullptr;

    RectTransformCornerSelectionGizmo();
	virtual ~RectTransformCornerSelectionGizmo();

    void UpdateBasedOnCornerSide();
};

NAMESPACE_BANG_EDITOR_END

#endif // RECTTRANSFORMCORNERSELECTIONGIZMO_H

