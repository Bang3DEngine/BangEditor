#include "BangEditor/RectTransformCornerSelectionGizmo.h"

#include "Bang/Color.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/Material.h"
#include "Bang/Rect.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Selection.h"

using namespace Bang;
using namespace BangEditor;

const int RectTransformCornerSelectionGizmo::CornerSize = 6;
const int RectTransformCornerSelectionGizmo::CornerSelectionSize = 8;

RectTransformCornerSelectionGizmo::RectTransformCornerSelectionGizmo()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    SetName("RectTransformCornerSelectionGizmo");

    p_cornerGO = GameObjectFactory::CreateUIGameObjectNamed("CornerGO");
    p_cornerRenderer = p_cornerGO->AddComponent<UIImageRenderer>();
    p_cornerRenderer->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);
    p_cornerRenderer->SetImageTexture(
        EditorTextureFactory::GetCircleHardIcon());

    p_selectionGO = GameObjectFactory::CreateUIGameObjectNamed("SelectionGO");
    p_selectionRenderer = p_selectionGO->AddComponent<UIImageRenderer>();
    p_selectionRenderer->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);

    p_cornerGO->SetParent(this);
    p_selectionGO->SetParent(this);
}

RectTransformCornerSelectionGizmo::~RectTransformCornerSelectionGizmo()
{
}

void RectTransformCornerSelectionGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject();
    if (!refGo)
    {
        return;
    }
    RectTransform *refRT = refGo->GetRectTransform();
    if (!refRT)
    {
        return;
    }

    if (IsBeingGrabbed())
    {
        Vector2i mousePos = Input::GetMousePosition();
        if (GrabHasJustChanged())
        {
            m_startGrabMousePos = mousePos;
            m_startMarginLeftBot = refRT->GetMarginLeftBot();
            m_startMarginRightTop = refRT->GetMarginRightTop();
        }

        Vector2 startGrabMousePosLocal =
            refRT->FromViewportPointToLocalPoint(m_startGrabMousePos);
        Vector2 mousePosLocal = refRT->FromViewportPointToLocalPoint(mousePos);

        Vector2 displacement(mousePos - m_startGrabMousePos);
        Vector2 displacementLocal = mousePosLocal - startGrabMousePosLocal;
        Vector2 dispDir = displacementLocal.NormalizedSafe();
        displacement = dispDir * displacement.Length();

        Vector2i newMarginLeftBot = m_startMarginLeftBot;
        Vector2i newMarginRightTop = m_startMarginRightTop;
        switch (m_cornerSide)
        {
            case CornerSide::LEFT_BOT:
                newMarginLeftBot.x += displacement.x;
                newMarginLeftBot.y += displacement.y;
                break;

            case CornerSide::LEFT_TOP:
                newMarginLeftBot.x += displacement.x;
                newMarginRightTop.y -= displacement.y;
                break;

            case CornerSide::RIGHT_TOP:
                newMarginRightTop.x -= displacement.x;
                newMarginRightTop.y -= displacement.y;
                break;

            case CornerSide::RIGHT_BOT:
                newMarginRightTop.x -= displacement.x;
                newMarginLeftBot.y += displacement.y;
                break;

            case CornerSide::CENTER:
                newMarginLeftBot += Vector2i(displacement);
                newMarginRightTop -= Vector2i(displacement);
                break;
        }

        if (Input::GetKey(Key::LSHIFT))
        {
            constexpr int Snapping = 5;
            newMarginLeftBot = (newMarginLeftBot / Snapping) * Snapping;
            newMarginRightTop = (newMarginRightTop / Snapping) * Snapping;
        }

        refRT->SetMarginLeftBot(newMarginLeftBot);
        refRT->SetMarginRightTop(newMarginRightTop);
    }
}

void RectTransformCornerSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    UpdateBasedOnCornerSide();

    p_selectionRenderer->SetEnabled(Selection::IsBeingRendered());

    Color color;
    switch (GetSelectionState())
    {
        case SelectionGizmo::SelectionState::IDLE: color = Color::Blue; break;

        case SelectionGizmo::SelectionState::OVER: color = Color::Orange; break;

        case SelectionGizmo::SelectionState::GRABBED:
            color = Color::Yellow;
            break;
    }
    p_cornerRenderer->GetMaterial()->SetAlbedoColor(color);

    SelectionGizmo::Render(renderPass, renderChildren);
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
    GameObject *refGo = GetReferencedGameObject();
    if (!refGo)
    {
        return;
    }
    RectTransform *refRT = refGo->GetRectTransform();
    if (!refRT)
    {
        return;
    }

    Rect refRect = refRT->GetViewportRect();
    Vector2 leftBotNDC(
        GL::FromViewportPointToViewportPointNDC(refRect.GetLeftBot()));
    Vector2 leftTopNDC(
        GL::FromViewportPointToViewportPointNDC(refRect.GetLeftTop()));
    Vector2 rightTopNDC(
        GL::FromViewportPointToViewportPointNDC(refRect.GetRightTop()));
    Vector2 rightBotNDC(
        GL::FromViewportPointToViewportPointNDC(refRect.GetRightBot()));
    Vector2 centerNDC(
        GL::FromViewportPointToViewportPointNDC(refRect.GetCenter()));

    Vector2 cornerAnchor = Vector2::Zero;
    switch (m_cornerSide)
    {
        case CornerSide::LEFT_BOT:
            cornerAnchor = Vector2(leftBotNDC.x, leftBotNDC.y);
            break;

        case CornerSide::LEFT_TOP:
            cornerAnchor = Vector2(leftTopNDC.x, leftTopNDC.y);
            break;

        case CornerSide::RIGHT_TOP:
            cornerAnchor = Vector2(rightTopNDC.x, rightTopNDC.y);
            break;

        case CornerSide::RIGHT_BOT:
            cornerAnchor = Vector2(rightBotNDC.x, rightBotNDC.y);
            break;

        case CornerSide::CENTER:
            cornerAnchor = Vector2(centerNDC.x, centerNDC.y);
            break;
    }

    // Update corner and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_cornerGO->GetRectTransform()
                                     : p_selectionGO->GetRectTransform();

        rt->SetAnchorX(Vector2(cornerAnchor.x));
        rt->SetAnchorY(Vector2(cornerAnchor.y));

        rt->SetMargins(-((i == 0) ? CornerSize : CornerSelectionSize));
    }
}
