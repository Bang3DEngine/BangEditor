#ifndef RECTTRANSFORMSELECTIONGIZMO_H
#define RECTTRANSFORMSELECTIONGIZMO_H

#include "Bang/GameObject.h"

#include "BangEditor/SelectionGizmo.h"

NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RectTransformCornerSelectionGizmo;

class RectTransformSelectionGizmo : public GameObject,
                                    public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RectTransformSelectionGizmo);

public:
    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    GameObject *p_selectionGo = nullptr;
    RectTransformCornerSelectionGizmo *p_botLeftCorner  = nullptr;
    RectTransformCornerSelectionGizmo *p_topLeftCorner  = nullptr;
    RectTransformCornerSelectionGizmo *p_topRightCorner = nullptr;
    RectTransformCornerSelectionGizmo *p_botRightCorner = nullptr;

	RectTransformSelectionGizmo();
	virtual ~RectTransformSelectionGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // RECTTRANSFORMSELECTIONGIZMO_H

