#include "BangEditor/AESConnectionLine.h"

#include <sys/types.h>
#include <memory>

#include "Bang/Animator.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachinePlayer.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/DPtr.tcc"
#include "Bang/GL.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Geometry.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/Math.h"
#include "Bang/MouseButton.h"
#include "Bang/RectTransform.h"
#include "Bang/RenderPass.h"
#include "Bang/TextureFactory.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITheme.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/AnimatorSMEditorScene.h"
#include "BangEditor/GIWAESConnectionLine.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

using namespace Bang;
using namespace BangEditor;

AESConnectionLine::AESConnectionLine()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::CANVAS);
    p_lineRenderer->GetMaterial()->SetRenderPass(RenderPass::CANVAS);
    p_lineRenderer->SetPoints({Vector3::Zero, Vector3::Zero});

    for (uint i : {0u, 1u, 2u})
    {
        GameObject *arrowImgGo = GameObjectFactory::CreateUIGameObject();
        p_arrowImgs[i] = arrowImgGo->AddComponent<UIImageRenderer>();
        p_arrowImgs[i]->SetImageTexture(TextureFactory::GetRightArrowIcon());
        arrowImgGo->GetRectTransform()->SetAnchors(Vector2::Zero);
        arrowImgGo->GetRectTransform()->SetSizeFromPivot(Vector2i(20));
        arrowImgGo->GetRectTransform()->SetPivotPosition(Vector2::Zero);
        arrowImgGo->SetParent(this);
    }

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem) {
        menuRootItem->SetFontSize(12);

        MenuItem *menuItem = menuRootItem->AddItem("Remove Transition");
        menuItem->SetSelectedCallback([this](MenuItem *) { RemoveSelf(); });
    });
}

AESConnectionLine::~AESConnectionLine()
{
}

void AESConnectionLine::BeforeRender()
{
    GameObject::BeforeRender();

    float lineWidth = 2.0f;
    Color lineColor = Color::White;
    AESNode *provisionalToNode = nullptr;
    if (!GetAESNodeTo())
    {
        if (UICanvas *canvas = UICanvas::GetActive(this))
        {
            if (UIFocusable *overedFocus =
                    canvas->GetFocusableUnderMouseTopMost())
            {
                GameObject *overedGo = overedFocus->GetGameObject();
                if (auto overedNode = DCAST<AESNode *>(overedGo))
                {
                    if (IsValidConnection(GetAESNodeFrom(), overedNode))
                    {
                        provisionalToNode = overedNode;
                    }
                }
            }
        }
    }
    else
    {
        bool thickenLine = false;
        if (IsMouseOver())
        {
            thickenLine = true;
            if (Input::GetMouseButtonDown(MouseButton::RIGHT))
            {
                p_contextMenu->ShowMenu();
            }

            if (Input::GetMouseButtonDown(MouseButton::RIGHT) ||
                Input::GetMouseButtonDown(MouseButton::LEFT))
            {
                if (UICanvas *canvas = UICanvas::GetActive(this))
                {
                    canvas->SetFocus(nullptr);
                }

                m_hasFocus = true;

                GIWAESConnectionLine *aesConnLine = new GIWAESConnectionLine();
                aesConnLine->SetAESConnectionLine(this);
                aesConnLine->Init();
                Inspector::GetActive()->ShowInspectorWidget(aesConnLine);
            }
        }
        else
        {
            if (Input::GetMouseButtonDown(MouseButton::LEFT))
            {
                m_hasFocus = false;
            }
        }

        if (thickenLine || m_hasFocus)
        {
            lineWidth = 5.0f;
            lineColor = m_hasFocus ? UITheme::GetSelectedColor()
                                   : UITheme::GetOverColor();
        }
    }

    if (Animator *animator = GetAESNodeFrom()->GetCurrentAnimator())
    {
        if (AnimatorStateMachinePlayer *player = animator->GetPlayer())
        {
            if (player->GetCurrentTransition() &&
                GetSMConnections().Contains(player->GetCurrentTransition()))
            {
                lineColor = Color::Green;
            }
        }
    }

    // Set line points positions
    {
        Vector2 mousePosition(Input::GetMousePosition());
        Vector3 lineMousePos = GetRectTransform()->FromWorldToLocalPoint(
            Vector3(mousePosition, 0.0f));
        uint i = 0;
        for (AESNode *node :
             {GetAESNodeFrom(),
              (GetAESNodeTo() ? GetAESNodeTo() : provisionalToNode)})
        {
            Vector3 linePos =
                node ? GetConnectionPointLinePosition(node) : lineMousePos;
            linePos.z = 0.0f;
            p_lineRenderer->SetPoint(i, linePos);
            ++i;
        }
        OffsetLinePositions();
    }

    if (HasFocus())
    {
        if (Input::GetKeyDown(Key::DELETE))
        {
            RemoveSelf();
        }
    }

    for (UIImageRenderer *arrowImg : p_arrowImgs)
    {
        arrowImg->GetMaterial()->SetAlbedoColor(lineColor);
    }
    p_lineRenderer->GetMaterial()->SetAlbedoColor(lineColor);
    p_lineRenderer->GetMaterial()->SetLineWidth(lineWidth);

    for (uint i : {0u, 1u, 2u})
    {
        RectTransform *arrowRT =
            p_arrowImgs[i]->GetGameObject()->GetRectTransform();

        Vector2 p0 = p_lineRenderer->GetPoints()[0].xy();
        Vector2 p1 = p_lineRenderer->GetPoints()[1].xy();

        constexpr float iLineStep = 0.05f;
        Vector2 pointForArrow =
            p0 + (p1 - p0) * ((0.5f - iLineStep) + (i * iLineStep));
        arrowRT->SetAnchors(pointForArrow);

        Vector2 p0w =
            GetRectTransform()->FromLocalToWorldPoint(Vector3(p0, 0)).xy();
        Vector2 p1w =
            GetRectTransform()->FromLocalToWorldPoint(Vector3(p1, 0)).xy();
        float angle = Math::ATan2((p1w - p0w).x, (p1w - p0w).y);
        angle += (Math::Pi * 1.5f);
        Quaternion rotation = Quaternion::AngleAxis(angle, Vector3::Forward);
        arrowRT->SetLocalRotation(rotation);

        if (i != 1)
        {
            p_arrowImgs[i]->SetEnabled(GetSMConnections().Size() >= 2);
        }
    }
}

void AESConnectionLine::SetNodeTo(AESNode *connPointTo)
{
    p_aesNodeTo = connPointTo;
}

void AESConnectionLine::SetNodeFrom(AESNode *connPointFrom)
{
    p_aesNodeFrom = connPointFrom;
}

bool AESConnectionLine::HasFocus() const
{
    return m_hasFocus;
}

AESNode *AESConnectionLine::GetAESNodeTo() const
{
    return p_aesNodeTo;
}

AESNode *AESConnectionLine::GetAESNodeFrom() const
{
    return p_aesNodeFrom;
}

AnimatorStateMachine *AESConnectionLine::GetAnimatorSM() const
{
    if (GetAESNodeFrom())
    {
        return GetAESNodeFrom()->GetAnimatorSM();
    }
    return GetAESNodeTo() ? GetAESNodeTo()->GetAnimatorSM() : nullptr;
}

Array<AnimatorStateMachineConnection *> AESConnectionLine::GetSMConnections()
    const
{
    Array<AnimatorStateMachineConnection *> connections;
    if (GetAESNodeFrom() && GetAESNodeTo())
    {
        AnimatorStateMachineNode *smNodeFrom = GetAESNodeFrom()->GetSMNode();
        AnimatorStateMachineNode *smNodeTo = GetAESNodeTo()->GetSMNode();
        connections = smNodeFrom->GetConnectionsTo(smNodeTo);
    }
    return connections;
}

bool AESConnectionLine::IsMouseOver() const
{
    if (!GetAESNodeFrom() || !GetAESNodeTo() || !IsActiveRecursively() ||
        !IsVisibleRecursively())
    {
        return false;
    }

    Vector2 linePosFrom =
        GetRectTransform()
            ->FromLocalToWorldPoint(p_lineRenderer->GetPoints()[0])
            .xy();
    Vector2 linePosTo =
        GetRectTransform()
            ->FromLocalToWorldPoint(p_lineRenderer->GetPoints()[1])
            .xy();
    Vector2 mousePos(Input::GetMousePosition());

    bool interactingWithNodeTo =
        GetAESNodeTo() && (GetAESNodeTo()->GetFocusable()->IsMouseOver() ||
                           GetAESNodeTo()->GetFocusable()->IsBeingPressed());
    bool interactingWithNodeFrom =
        GetAESNodeFrom() &&
        (GetAESNodeFrom()->GetFocusable()->IsMouseOver() ||
         GetAESNodeFrom()->GetFocusable()->IsBeingPressed());

    float distanceToLine =
        Geometry::GetPointToLineDistance2D(mousePos, linePosFrom, linePosTo);
    return (distanceToLine < 8.0f) &&
           Vector2::Dot(mousePos - linePosFrom, linePosTo - linePosFrom) > 0 &&
           Vector2::Dot(mousePos - linePosTo, linePosFrom - linePosTo) > 0 &&
           !interactingWithNodeTo && !interactingWithNodeFrom;
}

void AESConnectionLine::RemoveSelf()
{
    AnimatorStateMachineNode *smNode = GetAESNodeFrom()->GetSMNode();
    ASSERT(smNode);

    MetaNode previousMeta = smNode->GetMeta();

    const auto connections = GetSMConnections();
    for (AnimatorStateMachineConnection *smConn : connections)
    {
        smNode->RemoveConnection(smConn);
    }

    UndoRedoManager::PushAction(new UndoRedoSerializableChange(
        smNode, previousMeta, smNode->GetMeta()));
}

AESNode *AESConnectionLine::GetFirstFoundNode() const
{
    if (GetAESNodeTo())
    {
        return GetAESNodeTo();
    }
    return GetAESNodeFrom();
}

bool AESConnectionLine::IsValidConnection(AESNode *oneNode,
                                          AESNode *otherNode) const
{
    if (oneNode && otherNode)
    {
        return (oneNode != otherNode);
    }
    return false;
}

Vector3 AESConnectionLine::GetConnectionPointLinePosition(AESNode *node) const
{
    RectTransform *nodeRT = node->GetRectTransform();

    Vector3 linePos = GetRectTransform()->FromWorldToLocalPoint(
        nodeRT->FromLocalToWorldPoint(Vector3(0, 0, 0)));
    return linePos;
}

void AESConnectionLine::OffsetLinePositions() const
{
    bool isLesserNode = GetAESNodeTo()
                            ? (GetAESNodeFrom()->GetIndexInStateMachine() <
                               GetAESNodeTo()->GetIndexInStateMachine())
                            : true;

    Vector3 lineFromPos = p_lineRenderer->GetPoints()[0];
    Vector3 lineToPos = p_lineRenderer->GetPoints()[1];
    Vector3 lineFromTo =
        (lineToPos - lineFromPos) * (isLesserNode ? 1.0f : -1.0f);

    const float zoomScale = GetAESNodeFrom()->GetAESScene()->GetZoomScale();
    const int offsetPx = (30 * zoomScale);
    Vector3 offsetAnchorPerIndex(
        (1.0f / GetRectTransform()->GetViewportAARect().GetSize()) *
            Vector2(offsetPx),
        0.0f);

    Vector3 offsetDir = Vector3(lineFromTo.xy().Perpendicular(), 0.0f);
    offsetDir = offsetDir.NormalizedSafe();

    if (GetAESNodeFrom())
    {
        float sign = (isLesserNode ? 1.0f : -1.0f);
        lineFromPos += offsetDir * offsetAnchorPerIndex * sign;
        p_lineRenderer->SetPoint(0, lineFromPos);
    }

    if (GetAESNodeTo())
    {
        float sign = (isLesserNode ? 1.0f : -1.0f);
        lineToPos += offsetDir * offsetAnchorPerIndex * sign;
        p_lineRenderer->SetPoint(1, lineToPos);
    }
}
