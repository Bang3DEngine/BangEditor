#ifndef RECTTRANSFORMSELECTIONGIZMO_H
#define RECTTRANSFORMSELECTIONGIZMO_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang {
class GameObject;
}  // namespace Bang

NAMESPACE_BANG_BEGIN
FORWARD class UIImageRenderer;

NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class RectTransformAnchorSelectionGizmo;
FORWARD class RectTransformCornerSelectionGizmo;

class RectTransformSelectionGizmo : public SelectionGizmo
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
    RectTransformCornerSelectionGizmo *p_leftBotCorner  = nullptr;
    RectTransformCornerSelectionGizmo *p_leftTopCorner  = nullptr;
    RectTransformCornerSelectionGizmo *p_rightTopCorner = nullptr;
    RectTransformCornerSelectionGizmo *p_rightBotCorner = nullptr;
    RectTransformCornerSelectionGizmo *p_centerCorner   = nullptr;

    RectTransformAnchorSelectionGizmo *p_leftBotAnchor  = nullptr;
    RectTransformAnchorSelectionGizmo *p_leftTopAnchor  = nullptr;
    RectTransformAnchorSelectionGizmo *p_rightTopAnchor = nullptr;
    RectTransformAnchorSelectionGizmo *p_rightBotAnchor = nullptr;

	RectTransformSelectionGizmo();
	virtual ~RectTransformSelectionGizmo();
};

NAMESPACE_BANG_EDITOR_END

#endif // RECTTRANSFORMSELECTIONGIZMO_H

