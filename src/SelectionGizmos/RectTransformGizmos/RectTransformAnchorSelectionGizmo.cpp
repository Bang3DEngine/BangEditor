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

const int RectTransformAnchorSelectionGizmo::AnchorSize = 3;
const int RectTransformAnchorSelectionGizmo::AnchorSelectionSize = 5;

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
            m_startLocalAnchorMin = refRT->GetAnchorMin();
            m_startLocalAnchorMax = refRT->GetAnchorMax();
        }

        Vector2 displacement = (mousePosNDC - m_startGrabMousePosNDC);
        Vector2 newLocalAnchorMin = m_startLocalAnchorMin;
        Vector2 newLocalAnchorMax = m_startLocalAnchorMax;

        switch (m_anchorSide)
        {
            case AnchorSide::LeftBot:
                newLocalAnchorMin.x += displacement.x;
                newLocalAnchorMin.y += displacement.y;
            break;

            case AnchorSide::LeftTop:
                newLocalAnchorMin.x += displacement.x;
                newLocalAnchorMax.y += displacement.y;
            break;

            case AnchorSide::RightTop:
                newLocalAnchorMax.x += displacement.x;
                newLocalAnchorMax.y += displacement.y;
            break;

            case AnchorSide::RightBot:
                newLocalAnchorMax.x += displacement.x;
                newLocalAnchorMin.y += displacement.y;
            break;
        }

        if (Input::GetKey(Key::LShift))
        {
            constexpr float Snapping = 10.0f;
            newLocalAnchorMin = Vector2::Round(newLocalAnchorMin * Snapping) / Snapping;
            newLocalAnchorMax = Vector2::Round(newLocalAnchorMax * Snapping) / Snapping;
        }

        refRT->SetAnchorMin( newLocalAnchorMin );
        refRT->SetAnchorMax( newLocalAnchorMax );
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

    float localAnchorRot;
    Vector2 localAnchor;
    switch (m_anchorSide)
    {
        case AnchorSide::LeftBot:
            localAnchor.x = refRT->GetAnchorMin().x;
            localAnchor.y = refRT->GetAnchorMin().y;
            localAnchorRot = 225.0f;
        break;

        case AnchorSide::LeftTop:
            localAnchor.x = refRT->GetAnchorMin().x;
            localAnchor.y = refRT->GetAnchorMax().y;
            localAnchorRot = -45.0f;
        break;

        case AnchorSide::RightTop:
            localAnchor.x = refRT->GetAnchorMax().x;
            localAnchor.y = refRT->GetAnchorMax().y;
            localAnchorRot = 45.0f;
        break;

        case AnchorSide::RightBot:
            localAnchor.x = refRT->GetAnchorMax().x;
            localAnchor.y = refRT->GetAnchorMin().y;
            localAnchorRot = 135.0f;
        break;
    }

    Vector2 vpAnchorNDC = localAnchor;
    Quaternion parentRot = Quaternion::Identity;
    GameObject *parent = refGo->GetParent();
    RectTransform *parentRT = parent ? parent->GetRectTransform() : nullptr;
    if (parentRT)
    {
        vpAnchorNDC = parentRT->FromLocalPointNDCToViewportPointNDC( localAnchor );
        parentRot = parentRT->GetRotation();
    }

    // Update anchor and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_anchorGO->GetRectTransform() :
                                       p_selectionGO->GetRectTransform();

        rt->SetAnchorX(Vector2(vpAnchorNDC.x));
        rt->SetAnchorY(Vector2(vpAnchorNDC.y));

        const int size = (  (i == 0) ? AnchorSize : AnchorSelectionSize );
        rt->SetMargins(-size, -size*2, -size, 0);
        rt->SetPivotPosition( Vector2(0, -1) );

        // Fit into screen if in borders
        if ( Math::Abs(vpAnchorNDC.x) >= 0.9f || Math::Abs(vpAnchorNDC.y) >= 0.9f)
        {
            localAnchorRot += 180.0f;
        }
        Quaternion anchorRot = Quaternion::AngleAxis(Math::DegToRad(localAnchorRot),
                                                     Vector3::Forward);
        anchorRot = anchorRot * parentRot;
        rt->SetLocalRotation(anchorRot);
    }
}

