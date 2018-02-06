#include "BangEditor/RectTransformAnchorSelectionGizmo.h"

#include "Bang/Input.h"
#include "Bang/AARect.h"
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

const int RectTransformAnchorSelectionGizmo::AnchorSize = 8;
const int RectTransformAnchorSelectionGizmo::AnchorSelectionSize = 16;

RectTransformAnchorSelectionGizmo::RectTransformAnchorSelectionGizmo()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    SetName("RectTransformAnchorSelectionGizmo");

    p_anchorGO = GameObjectFactory::CreateUIGameObjectNamed("AnchorGO");
    p_anchorRenderer = p_anchorGO->AddComponent<UIImageRenderer>();
    p_anchorRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);
    p_anchorRenderer->SetImageTexture(EditorIconManager::GetAnchorIcon().Get());

    p_selectionGO = GameObjectFactory::CreateUIGameObjectNamed("SelectionGO");
    p_selectionRenderer = p_selectionGO->AddComponent<UIImageRenderer>();
    p_selectionRenderer->GetMaterial()->SetRenderPass(RenderPass::Overlay);

    p_anchorGO->SetParent(this);
    p_selectionGO->SetParent(this);
}

RectTransformAnchorSelectionGizmo::~RectTransformAnchorSelectionGizmo()
{
}

void RectTransformAnchorSelectionGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject(); if (!refGo) { return; }
    RectTransform *refRT = refGo->GetRectTransform(); if (!refRT) { return; }

    if (IsBeingGrabbed())
    {
        Vector2 mousePosNDC = Input::GetMousePositionNDC();
        if (GrabHasJustChanged())
        {
            m_startGrabMousePosNDC = mousePosNDC;
            m_startAnchorMin = refRT->GetAnchorMin();
            m_startAnchorMax = refRT->GetAnchorMax();
        }

        Vector2 displacement = (mousePosNDC - m_startGrabMousePosNDC);
        Vector2 newAnchorMin = m_startAnchorMin;
        Vector2 newAnchorMax = m_startAnchorMax;

        switch (m_anchorSide)
        {
            case AnchorSide::LeftBot:
                newAnchorMin.x += displacement.x;
                newAnchorMin.y += displacement.y;
            break;

            case AnchorSide::LeftTop:
                newAnchorMin.x += displacement.x;
                newAnchorMax.y += displacement.y;
            break;

            case AnchorSide::RightTop:
                newAnchorMax.x += displacement.x;
                newAnchorMax.y += displacement.y;
            break;

            case AnchorSide::RightBot:
                newAnchorMax.x += displacement.x;
                newAnchorMin.y += displacement.y;
            break;
        }

        if (Input::GetKey(Key::LShift))
        {
            constexpr float Snapping = 10.0f;
            newAnchorMin = Vector2::Round(newAnchorMin * Snapping) / Snapping;
            newAnchorMax = Vector2::Round(newAnchorMax * Snapping) / Snapping;
        }

        refRT->SetAnchorMin( newAnchorMin );
        refRT->SetAnchorMax( newAnchorMax );
    }

    UpdateBasedOnAnchorSide();
}

void RectTransformAnchorSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    UpdateBasedOnAnchorSide();

    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionRenderer->SetEnabled(selection);

    SelectionGizmo::Render(renderPass, renderChildren);

    Color color;
    switch (GetSelectionState())
    {
        case SelectionGizmo::SelectionState::Idle:
            color = Color::White;
        break;

        case SelectionGizmo::SelectionState::Over:
            color = Color::Orange;
        break;

        case SelectionGizmo::SelectionState::Grabbed:
            color = Color::Yellow;
        break;
    }
    p_anchorRenderer->GetMaterial()->SetDiffuseColor(color);
}

void RectTransformAnchorSelectionGizmo::SetReferencedGameObject(
                                            GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
    UpdateBasedOnAnchorSide();
}

void RectTransformAnchorSelectionGizmo::SetAnchorSide(AnchorSide anchorSide)
{
    m_anchorSide = anchorSide;
    UpdateBasedOnAnchorSide();
}

void RectTransformAnchorSelectionGizmo::UpdateBasedOnAnchorSide()
{
    GameObject *refGo    = GetReferencedGameObject(); if (!refGo) { return; }
    RectTransform *refRT = refGo->GetRectTransform(); if (!refRT) { return; }

    float anchorX, anchorY, anchorRot;
    switch (m_anchorSide)
    {
        case AnchorSide::LeftBot:
            anchorX = refRT->GetAnchorMin().x;
            anchorY = refRT->GetAnchorMin().y;
            anchorRot = 225.0f;
        break;

        case AnchorSide::LeftTop:
            anchorX = refRT->GetAnchorMin().x;
            anchorY = refRT->GetAnchorMax().y;
            anchorRot = -45.0f;
        break;

        case AnchorSide::RightTop:
            anchorX = refRT->GetAnchorMax().x;
            anchorY = refRT->GetAnchorMax().y;
            anchorRot = 45.0f;
        break;

        case AnchorSide::RightBot:
            anchorX = refRT->GetAnchorMax().x;
            anchorY = refRT->GetAnchorMin().y;
            anchorRot = 135.0f;
        break;
    }

    // Update anchor and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_anchorGO->GetRectTransform() :
                                       p_selectionGO->GetRectTransform();

        rt->SetAnchorX(Vector2(anchorX));
        rt->SetAnchorY(Vector2(anchorY));

        int size = (  (i == 0) ? AnchorSize : AnchorSelectionSize );
        rt->SetMargins(-size, -size*2, -size, 0);
        rt->SetPivotPosition( Vector2(0, -1) );

        // Fit into screen if in borders
        if ( Math::Abs(anchorX) >= 0.9f || Math::Abs(anchorY) >= 0.9f)
        {
            anchorRot += 180.0f;
        }
        rt->SetRotation( Quaternion::AngleAxis(Math::DegToRad(anchorRot),
                                                          Vector3::Forward) );
    }
}

