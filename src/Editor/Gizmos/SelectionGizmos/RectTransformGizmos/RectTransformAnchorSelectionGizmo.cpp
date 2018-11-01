#include "BangEditor/RectTransformAnchorSelectionGizmo.h"

#include "Bang/Color.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/Material.h"
#include "Bang/Math.h"
#include "Bang/Quaternion.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/Vector2.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/Selection.h"

using namespace Bang;
using namespace BangEditor;

const int RectTransformAnchorSelectionGizmo::AnchorSize = 6;
const int RectTransformAnchorSelectionGizmo::AnchorSelectionSize = 8;

RectTransformAnchorSelectionGizmo::RectTransformAnchorSelectionGizmo()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    SetName("RectTransformAnchorSelectionGizmo");

    p_anchorGO = GameObjectFactory::CreateUIGameObjectNamed("AnchorGO");
    p_anchorRenderer = p_anchorGO->AddComponent<UIImageRenderer>();
    p_anchorRenderer->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);
    p_anchorRenderer->SetImageTexture(EditorTextureFactory::GetAnchorIcon());

    p_selectionGO = GameObjectFactory::CreateUIGameObjectNamed("SelectionGO");
    p_selectionRenderer = p_selectionGO->AddComponent<UIImageRenderer>();
    p_selectionRenderer->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);

    p_anchorGO->SetParent(this);
    p_selectionGO->SetParent(this);
}

RectTransformAnchorSelectionGizmo::~RectTransformAnchorSelectionGizmo()
{
}

void RectTransformAnchorSelectionGizmo::Update()
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
    RectTransform *parentRT = refGo->GetParent()->GetRectTransform();
    parentRT = parentRT ? parentRT : refRT;

    if (IsBeingGrabbed())
    {
        Vector2 mousePos(Input::GetMousePosition());
        if (GrabHasJustChanged())
        {
            m_startGrabMousePos = mousePos;
            m_startLocalAnchorMin = refRT->GetAnchorMin();
            m_startLocalAnchorMax = refRT->GetAnchorMax();
        }

        Vector2 startGrabMousePosLocal =
            parentRT->FromViewportPointToLocalPointNDC(m_startGrabMousePos);
        Vector2 mousePosLocal =
            parentRT->FromViewportPointToLocalPointNDC(mousePos);
        Vector2 displacementLocal = (mousePosLocal - startGrabMousePosLocal);
        Vector2 newAnchorMinLocal = m_startLocalAnchorMin;
        Vector2 newAnchorMaxLocal = m_startLocalAnchorMax;

        switch (m_anchorSide)
        {
            case AnchorSide::LEFT_BOT:
                newAnchorMinLocal.x += displacementLocal.x;
                newAnchorMinLocal.y += displacementLocal.y;
                break;

            case AnchorSide::LEFT_TOP:
                newAnchorMinLocal.x += displacementLocal.x;
                newAnchorMaxLocal.y += displacementLocal.y;
                break;

            case AnchorSide::RIGHT_TOP:
                newAnchorMaxLocal.x += displacementLocal.x;
                newAnchorMaxLocal.y += displacementLocal.y;
                break;

            case AnchorSide::RIGHT_BOT:
                newAnchorMaxLocal.x += displacementLocal.x;
                newAnchorMinLocal.y += displacementLocal.y;
                break;
        }

        if (Input::GetKey(Key::LSHIFT))
        {
            constexpr float Snapping = 10.0f;
            newAnchorMinLocal =
                Vector2::Round(newAnchorMinLocal * Snapping) / Snapping;
            newAnchorMaxLocal =
                Vector2::Round(newAnchorMaxLocal * Snapping) / Snapping;
        }

        refRT->SetAnchorMin(newAnchorMinLocal);
        refRT->SetAnchorMax(newAnchorMaxLocal);
    }

    UpdateBasedOnAnchorSide();
}

void RectTransformAnchorSelectionGizmo::Render(RenderPass renderPass,
                                               bool renderChildren)
{
    UpdateBasedOnAnchorSide();

    p_selectionRenderer->SetEnabled(Selection::IsBeingRendered());

    SelectionGizmo::Render(renderPass, renderChildren);

    Color color;
    switch (GetSelectionState())
    {
        case SelectionGizmo::SelectionState::IDLE: color = Color::White; break;

        case SelectionGizmo::SelectionState::OVER: color = Color::Orange; break;

        case SelectionGizmo::SelectionState::GRABBED:
            color = Color::Yellow;
            break;
    }
    p_anchorRenderer->GetMaterial()->SetAlbedoColor(color);
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

    float localAnchorRot = 0.0f;
    Vector2 localAnchor;
    switch (m_anchorSide)
    {
        case AnchorSide::LEFT_BOT:
            localAnchor.x = refRT->GetAnchorMin().x;
            localAnchor.y = refRT->GetAnchorMin().y;
            localAnchorRot = 225.0f;
            break;

        case AnchorSide::LEFT_TOP:
            localAnchor.x = refRT->GetAnchorMin().x;
            localAnchor.y = refRT->GetAnchorMax().y;
            localAnchorRot = -45.0f;
            break;

        case AnchorSide::RIGHT_TOP:
            localAnchor.x = refRT->GetAnchorMax().x;
            localAnchor.y = refRT->GetAnchorMax().y;
            localAnchorRot = 45.0f;
            break;

        case AnchorSide::RIGHT_BOT:
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
        vpAnchorNDC =
            parentRT->FromLocalPointNDCToViewportPointNDC(localAnchor);
        parentRot = parentRT->GetRotation();
    }

    // Update anchor and selection rectTransforms
    for (int i = 0; i < 2; ++i)
    {
        RectTransform *rt = (i == 0) ? p_anchorGO->GetRectTransform()
                                     : p_selectionGO->GetRectTransform();

        rt->SetAnchorX(Vector2(vpAnchorNDC.x));
        rt->SetAnchorY(Vector2(vpAnchorNDC.y));

        const int size = ((i == 0) ? AnchorSize : AnchorSelectionSize);
        rt->SetMargins(-size, -size * 2, -size, 0);
        rt->SetPivotPosition(Vector2(0, -1));

        // Fit into screen if in borders
        if (Math::Abs(vpAnchorNDC.x) >= 0.9f ||
            Math::Abs(vpAnchorNDC.y) >= 0.9f)
        {
            localAnchorRot += 180.0f;
        }
        Quaternion anchorRot = Quaternion::AngleAxis(
            Math::DegToRad(localAnchorRot), Vector3::Forward);
        anchorRot = anchorRot * parentRot;
        rt->SetLocalRotation(anchorRot);
    }
}
