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

class RectTransformCornerSelectionGizmo : public GameObject,
                                          public SelectionGizmo
{
    GAMEOBJECT_EDITOR(RectTransformCornerSelectionGizmo);

public:
    // GameObject
    void Update() override;
    void Render(RenderPass renderPass, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

    void SetSides(Side hSide, Side vSide);

private:
    static const int CornerSize;
    static const int CornerSelectionSize;

    Side m_hSide = Undef<Side>();
    Side m_vSide = Undef<Side>();

    GameObject *p_cornerGO = nullptr;
    GameObject *p_selectionGO = nullptr;
    UIImageRenderer *p_cornerRenderer = nullptr;
    UIImageRenderer *p_selectionRenderer = nullptr;

    RectTransformCornerSelectionGizmo();
	virtual ~RectTransformCornerSelectionGizmo();

    void UpdateBasedOnSides();
};

NAMESPACE_BANG_EDITOR_END

#endif // RECTTRANSFORMCORNERSELECTIONGIZMO_H

