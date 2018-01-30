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

const int RectTransformCornerSelectionGizmo::CornerSize = 5;
const int RectTransformCornerSelectionGizmo::CornerSelectionSize = 10;

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

        switch (m_cornerSide)
        {
            case CornerSide::LeftBot:
                newMarginLeftBot.x += displacement.x;
                newMarginLeftBot.y += displacement.y;
            break;

            case CornerSide::LeftTop:
                newMarginLeftBot.x += displacement.x;
                newMarginRightTop.y -= displacement.y;
            break;

            case CornerSide::RightTop:
                newMarginRightTop.x -= displacement.x;
                newMarginRightTop.y -= displacement.y;
            break;

            case CornerSide::RightBot:
                newMarginRightTop.x -= displacement.x;
                newMarginLeftBot.y += displacement.y;
            break;

            case CornerSide::Center:
                newMarginLeftBot.x  += displacement.x;
                newMarginLeftBot.y  += displacement.y;
                newMarginRightTop.x -= displacement.x;
                newMarginRightTop.y -= displacement.y;
            break;
        }

        refRT->SetMarginLeftBot( newMarginLeftBot );
        refRT->SetMarginRightTop( newMarginRightTop );
    }
}

void RectTransformCornerSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    UpdateBasedOnCornerSide();

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
    UpdateBasedOnCornerSide();
}

void RectTransformCornerSelectionGizmo::SetCornerSide(CornerSide cornerSide)
{
    m_cornerSide = cornerSide;
    UpdateBasedOnCornerSide();
}

void RectTransformCornerSelectionGizmo::UpdateBasedOnCornerSide()
{
    GameObject *refGo    = GetReferencedGameObject(); if (!refGo) { return; }
    RectTransform *refRT = refGo->GetRectTransform(); if (!refRT) { return; }

    Rect refRect = refRT->GetViewportRectNDC();

    float cornerAnchorX, cornerAnchorY;
    switch (m_cornerSide)
    {
        case CornerSide::LeftBot:
            cornerAnchorX = refRect.GetMin().x;
            cornerAnchorY = refRect.GetMin().y;
        break;

        case CornerSide::LeftTop:
            cornerAnchorX = refRect.GetMin().x;
            cornerAnchorY = refRect.GetMax().y;
        break;

        case CornerSide::RightTop:
            cornerAnchorX = refRect.GetMax().x;
            cornerAnchorY = refRect.GetMax().y;
        break;

        case CornerSide::RightBot:
            cornerAnchorX = refRect.GetMax().x;
            cornerAnchorY = refRect.GetMin().y;
        break;

        case CornerSide::Center:
            cornerAnchorX = refRect.GetCenter().x;
            cornerAnchorY = refRect.GetCenter().y;
        break;
    }

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

