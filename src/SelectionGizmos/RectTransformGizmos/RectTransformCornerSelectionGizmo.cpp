#include "BangEditor/RectTransformCornerSelectionGizmo.h"

#include "Bang/Rect.h"
#include "Bang/Gizmos.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

const int RectTransformCornerSelectionGizmo::CornerSize = 5;
const int RectTransformCornerSelectionGizmo::CornerSelectionSize = 10;

RectTransformCornerSelectionGizmo::RectTransformCornerSelectionGizmo()
{
    SetName("RectTransformCornerSelectionGizmo");

    GameObjectFactory::CreateUIGameObjectInto(this);
    UICanvas *canvas = GameObjectFactory::CreateUICanvasInto(this); (void)(canvas);

    p_cornerGO = GameObjectFactory::CreateUIGameObjectNamed("CornerGO");
    p_cornerRenderer = AddComponent<UIImageRenderer>();
    p_cornerRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);

    p_selectionGO = GameObjectFactory::CreateUIGameObjectNamed("SelectionGO");
    p_selectionRenderer = AddComponent<UIImageRenderer>();
    p_selectionRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);

    p_cornerGO->SetParent(this);
    p_selectionGO->SetParent(this);
}

RectTransformCornerSelectionGizmo::~RectTransformCornerSelectionGizmo()
{
}

void RectTransformCornerSelectionGizmo::Update()
{
    GameObject::Update();
}

void RectTransformCornerSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionRenderer->SetEnabled(selection);
    p_selectionRenderer->SetEnabled(false);

    GameObject::Render(renderPass, renderChildren);
}

void RectTransformCornerSelectionGizmo::SetReferencedGameObject(
                                            GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    UpdateBasedOnSides();
}

void RectTransformCornerSelectionGizmo::SetSides(Side hSide, Side vSide)
{
    ASSERT_SOFT(m_hSide == Side::Left || m_hSide == Side::Right);
    ASSERT_SOFT(m_vSide == Side::Top  || m_vSide == Side::Bot);

    m_hSide = hSide;
    m_vSide = vSide;

    UpdateBasedOnSides();
}

void RectTransformCornerSelectionGizmo::UpdateBasedOnSides()
{
    float anchorX = (m_hSide == Side::Left) ? -1.0f : 1.0f;
    float anchorY = (m_vSide == Side::Bot)  ? -1.0f : 1.0f;

    // Update corner and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_cornerGO->GetRectTransform() :
                                       p_selectionGO->GetRectTransform();

        rt->SetAnchorX(Vector2(anchorX));
        rt->SetAnchorY(Vector2(anchorY));

        rt->SetMargins( -( (i == 0) ? CornerSize : CornerSelectionSize ) );
    }
}

