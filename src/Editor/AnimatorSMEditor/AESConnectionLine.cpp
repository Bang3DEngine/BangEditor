#include "BangEditor/AESConnectionLine.h"

#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/Geometry.h"
#include "Bang/UIFocusable.h"
#include "Bang/LineRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESNode.h"
#include "BangEditor/UIContextMenu.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESConnectionLine::AESConnectionLine()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetViewProjMode(GL::ViewProjMode::CANVAS);
    p_lineRenderer->GetMaterial()->SetRenderPass(RenderPass::CANVAS);
    p_lineRenderer->SetPoints({Vector3::Zero, Vector3::Zero});

    GameObject *arrowImgGo = GameObjectFactory::CreateUIGameObject();
    p_arrowImg = arrowImgGo->AddComponent<UIImageRenderer>();
    p_arrowImg->SetImageTexture( TextureFactory::GetRightArrowIcon() );
    arrowImgGo->GetRectTransform()->SetAnchors(Vector2::Zero);
    arrowImgGo->GetRectTransform()->SetSizeFromPivot( Vector2i(20) );
    arrowImgGo->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    arrowImgGo->SetParent(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        menuRootItem->SetFontSize(12);

        MenuItem *menuItem = menuRootItem->AddItem("Remove Transition");
        menuItem->SetSelectedCallback([this](MenuItem*)
        {
            RemoveSelf();
        });
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
    if (!GetNodeTo())
    {
        if (UICanvas *canvas = UICanvas::GetActive(this))
        {
            if (UIFocusable *overedFocus = canvas->GetFocusableUnderMouseTopMost())
            {
                GameObject *overedGo = overedFocus->GetGameObject();
                if (auto overedNode = DCAST<AESNode*>(overedGo))
                {
                    if ( IsValidConnection(GetNodeFrom(), overedNode) )
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
            lineColor = UITheme::GetSelectedColor();
        }
    }

    // Set line points positions
    {
        Vector2 mousePosition ( Input::GetMousePosition() );
        Vector3 lineMousePos = GetRectTransform()->FromWorldToLocalPoint(
                                                Vector3(mousePosition, 0.0f));
        uint i = 0;
        for (AESNode *node : {GetNodeFrom(),
                             (GetNodeTo() ? GetNodeTo() : provisionalToNode)})
        {
            Vector3 linePos = node ? GetConnectionPointLinePosition(node) :
                                     lineMousePos;
            linePos.z = 0.0f;
            p_lineRenderer->SetPoint(i, linePos);
            ++i;
        }
    }

    if (HasFocus())
    {
        if (Input::GetKeyDown(Key::DELETE))
        {
            GameObject::Destroy(this);
        }
    }

    p_arrowImg->GetMaterial()->SetAlbedoColor(lineColor);
    p_lineRenderer->GetMaterial()->SetAlbedoColor(lineColor);
    p_lineRenderer->GetMaterial()->SetLineWidth(lineWidth);

    if (RectTransform *arrowRT = p_arrowImg->GetGameObject()->GetRectTransform())
    {
        Vector2 p0 = p_lineRenderer->GetPoints()[0].xy();
        Vector2 p1 = p_lineRenderer->GetPoints()[1].xy();

        Vector2 midPoint = (p0 + p1) * 0.5f;
        arrowRT->SetAnchors(midPoint);

        Vector2 p0w = GetRectTransform()->FromLocalToWorldPoint( Vector3(p0, 0) ).xy();
        Vector2 p1w = GetRectTransform()->FromLocalToWorldPoint( Vector3(p1, 0) ).xy();
        float angle = Math::ATan2((p1w-p0w).x, (p1w-p0w).y);
        angle += (Math::Pi * 1.5f);
        Quaternion rotation = Quaternion::AngleAxis(angle, Vector3::Forward);
        arrowRT->SetLocalRotation(rotation);
    }
}

void AESConnectionLine::SetNodeTo(AESNode *connPointTo)
{
    p_nodeTo = connPointTo;
}

void AESConnectionLine::SetNodeFrom(AESNode *connPointFrom)
{
    p_nodeFrom = connPointFrom;
}

bool AESConnectionLine::HasFocus() const
{
    return m_hasFocus;
}

AESNode *AESConnectionLine::GetNodeTo() const
{
    return p_nodeTo;
}

AESNode *AESConnectionLine::GetNodeFrom() const
{
    return p_nodeFrom;
}

bool AESConnectionLine::IsMouseOver() const
{
    if (!GetNodeFrom() || !GetNodeTo())
    {
        return false;
    }

    Vector2 linePosFrom = GetRectTransform()->FromLocalToWorldPoint(
                            p_lineRenderer->GetPoints()[0]).xy();
    Vector2 linePosTo   = GetRectTransform()->FromLocalToWorldPoint(
                            p_lineRenderer->GetPoints()[1]).xy();
    Vector2 mousePos ( Input::GetMousePosition() );

    bool interactingWithNodeTo = GetNodeTo() &&
                                 (GetNodeTo()->GetFocusable()->IsMouseOver() ||
                                  GetNodeTo()->GetFocusable()->IsBeingPressed());
    bool interactingWithNodeFrom = GetNodeFrom() &&
                                  (GetNodeFrom()->GetFocusable()->IsMouseOver() ||
                                   GetNodeFrom()->GetFocusable()->IsBeingPressed());

    float distanceToLine = Geometry::GetPointToLineDistance2D(mousePos,
                                                              linePosFrom,
                                                              linePosTo);
    return (distanceToLine < 10.0f) &&
            Vector2::Dot(mousePos-linePosFrom, linePosTo-linePosFrom) > 0 &&
            Vector2::Dot(mousePos-linePosTo,   linePosFrom-linePosTo) > 0 &&
            !interactingWithNodeTo &&
            !interactingWithNodeFrom;
}

void AESConnectionLine::RemoveSelf()
{
    GameObject::Destroy(this);
}

AESNode *AESConnectionLine::GetFirstFoundNode() const
{
    if (GetNodeTo())
    {
        return GetNodeTo();
    }
    return GetNodeFrom();
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

Vector3 AESConnectionLine::GetConnectionPointLinePosition(
                                        AESNode *node) const
{
    Vector3 linePos = GetRectTransform()->FromWorldToLocalPoint(
                        node->GetRectTransform()->FromLocalToWorldPoint(
                            Vector3(0, 0, 0)));
    return linePos;
}
