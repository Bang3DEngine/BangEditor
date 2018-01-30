#include "BangEditor/RectTransformCornerSelectionGizmo.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Gizmos.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/SelectionFramebuffer.h"

#include "BangEditor/EditorIconManager.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

const int RectTransformCornerSelectionGizmo::CornerSize = 10;
const int RectTransformCornerSelectionGizmo::CornerSelectionSize = 20;

RectTransformCornerSelectionGizmo::RectTransformCornerSelectionGizmo()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    SetName("RectTransformCornerSelectionGizmo");

    p_cornerGO = GameObjectFactory::CreateUIGameObjectNamed("CornerGO");
    p_cornerRenderer = p_cornerGO->AddComponent<UIImageRenderer>();
    p_cornerRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);
    p_cornerRenderer->SetImageTexture(EditorIconManager::GetCircleHardIcon().Get());

    p_selectionGO = GameObjectFactory::CreateUIGameObjectNamed("SelectionGO");
    p_selectionRenderer = p_selectionGO->AddComponent<UIImageRenderer>();
    p_selectionRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);

    p_cornerGO->SetParent(this);
    p_selectionGO->SetParent(this);
}

RectTransformCornerSelectionGizmo::~RectTransformCornerSelectionGizmo()
{
}

void RectTransformCornerSelectionGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject(); if (!refGo) { return; }
    RectTransform *refRT = refGo->GetRectTransform(); if (!refRT) { return; }

    if (IsBeingGrabbed())
    {
        Vector2i mousePos = Input::GetMousePosition();
        if (GrabHasJustChanged())
        {
            m_startGrabMousePos = mousePos;
            m_startMarginLeftBot  = refRT->GetMarginLeftBot();
            m_startMarginRightTop = refRT->GetMarginRightTop();
        }

        Vector2i displacement = (mousePos - m_startGrabMousePos);
        Vector2i newMarginLeftBot  = m_startMarginLeftBot;
        Vector2i newMarginRightTop = m_startMarginRightTop;

        if (m_hSide == Side::Left) { newMarginLeftBot.x -= displacement.x; }
        else if (m_hSide == Side::Right) { newMarginRightTop.x -= displacement.x; }

        if (m_vSide == Side::Bot) { newMarginLeftBot.y -= displacement.y; }
        else if (m_vSide == Side::Top) { newMarginRightTop.y -= displacement.y; }

        refRT->SetMarginLeftBot( newMarginLeftBot );
        refRT->SetMarginRightTop( newMarginRightTop );
    }
}

void RectTransformCornerSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    UpdateBasedOnSides();

    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionRenderer->SetEnabled(selection);

    SelectionGizmo::Render(renderPass, renderChildren);

    Color color;
    switch (GetSelectionState())
    {
        case SelectionGizmo::SelectionState::Idle:
            color = Color::Blue;
        break;

        case SelectionGizmo::SelectionState::Over:
            color = Color::Orange;
        break;

        case SelectionGizmo::SelectionState::Grabbed:
            color = Color::Yellow;
        break;
    }
    p_cornerRenderer->GetMaterial()->SetDiffuseColor(color);
}

void RectTransformCornerSelectionGizmo::SetReferencedGameObject(
                                            GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    UpdateBasedOnSides();
}

void RectTransformCornerSelectionGizmo::SetSides(Side hSide, Side vSide)
{
    m_hSide = hSide;
    m_vSide = vSide;

    ASSERT_SOFT_MSG(m_hSide == Side::Left || m_hSide == Side::Right, "Wrong side!");
    ASSERT_SOFT_MSG(m_vSide == Side::Top  || m_vSide == Side::Bot, "Wrong side!");

    UpdateBasedOnSides();
}

void RectTransformCornerSelectionGizmo::UpdateBasedOnSides()
{
    GameObject *refGo    = GetReferencedGameObject(); if (!refGo) { return; }
    RectTransform *refRT = refGo->GetRectTransform(); if (!refRT) { return; }

    Rect refRect = refRT->GetViewportRectNDC();
    float cornerAnchorX = (m_hSide == Side::Left) ? refRect.GetMin().x :
                                                    refRect.GetMax().x;
    float cornerAnchorY = (m_vSide == Side::Bot)  ? refRect.GetMin().y :
                                                    refRect.GetMax().y;

    // Update corner and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_cornerGO->GetRectTransform() :
                                       p_selectionGO->GetRectTransform();

        static float time = 0.0f;
        time += Time::GetDeltaTime();
        rt->SetAnchorX(Vector2(cornerAnchorX));
        rt->SetAnchorY(Vector2(cornerAnchorY));

        rt->SetMargins( -( (i == 0) ? CornerSize : CornerSelectionSize ) );
    }
}

