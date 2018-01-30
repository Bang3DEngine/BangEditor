#include "BangEditor/RectTransformSelectionGizmo.h"

#include "Bang/Rect.h"
#include "Bang/Gizmos.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/RectTransformCornerSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RectTransformSelectionGizmo::RectTransformSelectionGizmo()
{
    SetName("RectTransformSelectionGizmo");

    AddComponent<Transform>();

    p_leftBotCorner  = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_leftTopCorner  = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_rightTopCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_rightBotCorner = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_centerCorner   = GameObject::Create<RectTransformCornerSelectionGizmo>();
    p_leftBotCorner->SetCornerSide ( RectTransformCornerSelectionGizmo::CornerSide::LeftBot  );
    p_leftTopCorner->SetCornerSide ( RectTransformCornerSelectionGizmo::CornerSide::LeftTop  );
    p_rightTopCorner->SetCornerSide( RectTransformCornerSelectionGizmo::CornerSide::RightTop );
    p_rightBotCorner->SetCornerSide( RectTransformCornerSelectionGizmo::CornerSide::RightBot );
    p_centerCorner->SetCornerSide  ( RectTransformCornerSelectionGizmo::CornerSide::Center   );

    p_selectionGo = GameObjectFactory::CreateUIGameObject(true);
    p_selectionGo->SetName("RectSelection");

    p_selectionGo->SetParent(this);
    p_leftBotCorner->SetParent(this);
    p_leftTopCorner->SetParent(this);
    p_rightTopCorner->SetParent(this);
    p_rightBotCorner->SetParent(this);
    p_centerCorner->SetParent(this);
}

RectTransformSelectionGizmo::~RectTransformSelectionGizmo()
{
}

void RectTransformSelectionGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject();
    if (!refGo || !refGo->GetTransform()) { return; }
}

void RectTransformSelectionGizmo::Render(RenderPass renderPass, bool renderChildren)
{
    GetTransform()->SetScale(Vector3::One);

    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionGo->SetEnabled(selection);

    if (renderPass == RenderPass::Overlay)
    {
        GameObject *refGo = GetReferencedGameObject();
        if (!refGo) { return; }

        // Gizmos rendering!
        Gizmos::Reset();

        // Rect
        Gizmos::SetSelectable(nullptr);
        Gizmos::SetThickness(2.0f);
        Gizmos::SetColor( Color::White );
        Gizmos::SetRenderPass(renderPass);
        Gizmos::RenderRect( refGo->GetRectTransform()->GetViewportRectNDC() );
    }

    SelectionGizmo::Render(renderPass, renderChildren);
}

void RectTransformSelectionGizmo::SetReferencedGameObject(
                                    GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_leftBotCorner->SetReferencedGameObject (referencedGameObject);
    p_leftTopCorner->SetReferencedGameObject (referencedGameObject);
    p_rightTopCorner->SetReferencedGameObject(referencedGameObject);
    p_rightBotCorner->SetReferencedGameObject(referencedGameObject);
    p_centerCorner->SetReferencedGameObject(referencedGameObject);
}
