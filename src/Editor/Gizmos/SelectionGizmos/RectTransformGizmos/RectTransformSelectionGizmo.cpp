#include "BangEditor/RectTransformSelectionGizmo.h"

#include "Bang/Color.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Rect.h"
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

    p_leftBotCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_leftTopCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_rightTopCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_rightBotCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_centerCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
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

    p_leftBotAnchor = GameObject::Create<RectTransformAnchorSelectionGizmo>();
    p_leftTopAnchor = GameObject::Create<RectTransformAnchorSelectionGizmo>();
    p_rightTopAnchor = GameObject::Create<RectTransformAnchorSelectionGizmo>();
    p_rightBotAnchor = GameObject::Create<RectTransformAnchorSelectionGizmo>();
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
    if(!refGo || !refGo->GetTransform())
    {
        return;
    }
}

void RectTransformSelectionGizmo::Render(RenderPass renderPass,
                                         bool renderChildren)
{
    p_selectionGo->SetEnabled(Selection::IsBeingRendered());

    if(renderPass == RenderPass::OVERLAY)
    {
        GameObject *refGo = GetReferencedGameObject();
        if(!refGo)
        {
            return;
        }
        if(!refGo->GetRectTransform())
        {
            return;
        }

        RenderFactory::Parameters params;
        params.thickness = 2.0f;
        params.color = Color::White;
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
