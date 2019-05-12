#include "BangEditor/RectTransformSelectionGizmo.h"

#include "BangMath/Color.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "BangMath/Rect.h"
#include "Bang/RectTransform.h"
#include "Bang/RenderFactory.h"
#include "BangEditor/RectTransformAnchorSelectionGizmo.h"
#include "BangEditor/RectTransformCornerSelectionGizmo.h"
#include "BangEditor/Selection.h"

using namespace Bang;
using namespace BangEditor;

RectTransformSelectionGizmo::RectTransformSelectionGizmo()
{
    SetName("RectTransformSelectionGizmo");

    GameObjectFactory::CreateUIGameObjectInto(this);

    p_leftBotCorner = new RectTransformCornerSelectionGizmo();
    p_leftTopCorner = new RectTransformCornerSelectionGizmo();
    p_rightTopCorner = new RectTransformCornerSelectionGizmo();
    p_rightBotCorner = new RectTransformCornerSelectionGizmo();
    p_centerCorner = new RectTransformCornerSelectionGizmo();
    p_leftBotCorner->SetCornerSide(
        RectTransformCornerSelectionGizmo::CornerSide::LEFT_BOT);
    p_leftTopCorner->SetCornerSide(
        RectTransformCornerSelectionGizmo::CornerSide::LEFT_TOP);
    p_rightTopCorner->SetCornerSide(
        RectTransformCornerSelectionGizmo::CornerSide::RIGHT_TOP);
    p_rightBotCorner->SetCornerSide(
        RectTransformCornerSelectionGizmo::CornerSide::RIGHT_BOT);
    p_centerCorner->SetCornerSide(
        RectTransformCornerSelectionGizmo::CornerSide::CENTER);

    p_leftBotAnchor = new RectTransformAnchorSelectionGizmo();
    p_leftTopAnchor = new RectTransformAnchorSelectionGizmo();
    p_rightTopAnchor = new RectTransformAnchorSelectionGizmo();
    p_rightBotAnchor = new RectTransformAnchorSelectionGizmo();
    p_leftBotAnchor->SetAnchorSide(
        RectTransformAnchorSelectionGizmo::AnchorSide::LEFT_BOT);
    p_leftTopAnchor->SetAnchorSide(
        RectTransformAnchorSelectionGizmo::AnchorSide::LEFT_TOP);
    p_rightTopAnchor->SetAnchorSide(
        RectTransformAnchorSelectionGizmo::AnchorSide::RIGHT_TOP);
    p_rightBotAnchor->SetAnchorSide(
        RectTransformAnchorSelectionGizmo::AnchorSide::RIGHT_BOT);

    p_selectionGo = GameObjectFactory::CreateUIGameObject(true);
    p_selectionGo->SetName("RectSelection");
    p_selectionGo->SetVisible(false);

    p_selectionGo->SetParent(this);
    p_leftBotCorner->SetParent(this);
    p_leftTopCorner->SetParent(this);
    p_rightTopCorner->SetParent(this);
    p_rightBotCorner->SetParent(this);
    p_centerCorner->SetParent(this);

    p_leftBotAnchor->SetParent(this);
    p_leftTopAnchor->SetParent(this);
    p_rightTopAnchor->SetParent(this);
    p_rightBotAnchor->SetParent(this);
}

RectTransformSelectionGizmo::~RectTransformSelectionGizmo()
{
}

void RectTransformSelectionGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject();
    if (!refGo || !refGo->GetTransform())
    {
        return;
    }
}

void RectTransformSelectionGizmo::Render(RenderPass renderPass,
                                         bool renderChildren)
{
    if (renderPass == RenderPass::OVERLAY)
    {
        GameObject *refGo = GetReferencedGameObject();
        if (!refGo)
        {
            return;
        }
        if (!refGo->GetRectTransform())
        {
            return;
        }

        RenderFactory::Parameters params;
        params.thickness = 2.0f;
        params.color = Color::White();
        RenderFactory::RenderRect(
            refGo->GetRectTransform()->GetViewportRect().GetPoints(), params);
    }

    SelectionGizmo::Render(renderPass, renderChildren);
}

void RectTransformSelectionGizmo::SetReferencedGameObject(
    GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_leftBotCorner->SetReferencedGameObject(referencedGameObject);
    p_leftTopCorner->SetReferencedGameObject(referencedGameObject);
    p_rightTopCorner->SetReferencedGameObject(referencedGameObject);
    p_rightBotCorner->SetReferencedGameObject(referencedGameObject);
    p_centerCorner->SetReferencedGameObject(referencedGameObject);

    p_leftBotAnchor->SetReferencedGameObject(referencedGameObject);
    p_leftTopAnchor->SetReferencedGameObject(referencedGameObject);
    p_rightTopAnchor->SetReferencedGameObject(referencedGameObject);
    p_rightBotAnchor->SetReferencedGameObject(referencedGameObject);
}
