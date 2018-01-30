#include "BangEditor/RectTransformAnchorSelectionGizmo.h"

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

const int RectTransformAnchorSelectionGizmo::AnchorSize = 5;
const int RectTransformAnchorSelectionGizmo::AnchorSelectionSize = 10;

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
                newAnchorMax.y -= displacement.y;
            break;

            case AnchorSide::RightTop:
                newAnchorMax.x -= displacement.x;
                newAnchorMax.y -= displacement.y;
            break;

            case AnchorSide::RightBot:
                newAnchorMax.x -= displacement.x;
                newAnchorMin.y += displacement.y;
            break;
        }

        refRT->SetAnchorMin( newAnchorMin );
        refRT->SetAnchorMax( newAnchorMax );
    }
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

    Rect refRect = refRT->GetViewportRectNDC();

    float anchorX, anchorY, anchorRot;
    switch (m_anchorSide)
    {
        case AnchorSide::LeftBot:
            anchorX = refRect.GetMin().x;
            anchorY = refRect.GetMin().y;
            anchorRot = -45.0f;
        break;

        case AnchorSide::LeftTop:
            anchorX = refRect.GetMin().x;
            anchorY = refRect.GetMax().y;
            anchorRot =  45.0f;
        break;

        case AnchorSide::RightTop:
            anchorX = refRect.GetMax().x;
            anchorY = refRect.GetMax().y;
            anchorRot = -135.0f;
        break;

        case AnchorSide::RightBot:
            anchorX = refRect.GetMax().x;
            anchorY = refRect.GetMin().y;
            anchorRot =  135.0f;
        break;
    }
    static double time = 0.0;
    time += Time::GetDeltaTime() * 360.0f * 0.1;
    anchorRot = Math::Modf(time, 360.0);

    // Update anchor and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_anchorGO->GetRectTransform() :
                                       p_selectionGO->GetRectTransform();

        static float time = 0.0f;
        time += Time::GetDeltaTime();
        rt->SetAnchorX(Vector2(anchorX));
        rt->SetAnchorY(Vector2(anchorY));
        rt->SetRotation( Quaternion::AngleAxis(Math::DegToRad(anchorRot),
                                               Vector3::Forward) );

        rt->SetMargins( -( (i == 0) ? AnchorSize : AnchorSelectionSize ) );
        if (m_anchorSide == AnchorSide::LeftBot ||
            m_anchorSide == AnchorSide::RightTop)
        { rt->SetMargins(-5, -25, -5, -25); }
        if (m_anchorSide == AnchorSide::LeftTop ||
            m_anchorSide == AnchorSide::RightBot)
        { rt->SetMargins(-25, -5, -25, -5); }

        rt->SetPivotPosition( Vector2(0, -1) );
    }
}

