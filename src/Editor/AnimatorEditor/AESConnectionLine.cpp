#include "BangEditor/AESConnectionLine.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/LineRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESConnectionPoint.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESConnectionLine::AESConnectionLine()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    p_lineRend = AddComponent<LineRenderer>();
    p_lineRend->SetViewProjMode(GL::ViewProjMode::CANVAS);
    p_lineRend->GetMaterial()->SetRenderPass(RenderPass::CANVAS);
    p_lineRend->GetMaterial()->SetLineWidth(5.0f);
    p_lineRend->SetPoints({Vector3::Zero, Vector3::Zero});

}

AESConnectionLine::~AESConnectionLine()
{
}

void AESConnectionLine::BeforeRender()
{
    GameObject::BeforeRender();

    Color lineColor = Color::White;
    bool totallyConnected = (GetConnectionPointIn() && GetConnectionPointOut());
    if (totallyConnected && !p_currentDragOtherConnectionPoint)
    {
        // See if we are disconnecting from one of the connection points...
        if (GetConnectionPointIn()->GetFocusable()->IsBeingPressed())
        {
            SetConnectionPointIn(nullptr);
            totallyConnected = false;
        }
        else if (GetConnectionPointOut()->GetFocusable()->IsBeingPressed())
        {
            SetConnectionPointOut(nullptr);
            totallyConnected = false;
        }
        else
        {
            lineColor = Color::Green;
        }
    }

    if (!totallyConnected)
    {
        ASSERT(GetFirstFoundConnectedPoint());

        // Check drag
        const bool isDraggingLine = (Input::GetMouseButton(MouseButton::LEFT) &&
                                     !Input::GetMouseButton(MouseButton::MIDDLE));
        if (isDraggingLine)
        {
            p_currentDragOtherConnectionPoint = nullptr;
            if (UICanvas *canvas = UICanvas::GetActive(this))
            {
                if (UIFocusable *overedFocus = canvas->GetFocusableUnderMouseTopMost())
                {
                    GameObject *overedGo = overedFocus->GetGameObject();
                    if (auto overedConnPoint = DCAST<AESConnectionPoint*>(overedGo))
                    {
                        auto connectedPoint = GetFirstFoundConnectedPoint();
                        ASSERT(connectedPoint && overedConnPoint);

                        if ( IsValidConnection(connectedPoint, overedConnPoint) )
                        {
                            p_currentDragOtherConnectionPoint = overedConnPoint;
                            lineColor = Color::Green;
                        }
                        else
                        {
                            lineColor = Color::Red;
                        }
                    }
                }
            }
        }
        else
        {
            // If stopped dragging, connect or destroy
            if (p_currentDragOtherConnectionPoint)
            {
                AESConnectionPoint::Connect(GetFirstFoundConnectedPoint(),
                                            p_currentDragOtherConnectionPoint);
                if (GetConnectionPointIn())
                {
                    SetConnectionPointOut( p_currentDragOtherConnectionPoint );
                }
                else
                {
                    SetConnectionPointIn( p_currentDragOtherConnectionPoint );
                }
                p_currentDragOtherConnectionPoint = nullptr;
            }
            else
            {
                // Not totally connected and stopped dragging without final
                // target. Destroy connection line
                GameObject::Destroy(this);
            }
        }

    }
    p_lineRend->GetMaterial()->SetAlbedoColor(lineColor);

    // Set line points positions
    {
        Vector2 mousePosition ( Input::GetMousePosition() );
        Vector3 lineMousePos = GetRectTransform()->FromWorldToLocalPoint(
                                                Vector3(mousePosition, -0.02f));
        uint i = 0;
        for (AESConnectionPoint *connPoint : {GetConnectionPointIn(),
                                              GetConnectionPointOut()})
        {
            if (connPoint)
            {
                Vector3 linePos = GetConnectionPointLinePosition(connPoint);
                p_lineRend->SetPoint(i, linePos);
            }
            else
            {
                p_lineRend->SetPoint(i, lineMousePos);
            }
            ++i;
        }
    }
}

void AESConnectionLine::SetConnectionPointIn(AESConnectionPoint *connPointIn)
{
    p_connectionPointIn = connPointIn;
}

void AESConnectionLine::SetConnectionPointOut(AESConnectionPoint *connPointOut)
{
    p_connectionPointOut = connPointOut;
}

AESConnectionPoint *AESConnectionLine::GetConnectionPointIn() const
{
    return p_connectionPointIn;
}

AESConnectionPoint *AESConnectionLine::GetConnectionPointOut() const
{
    return p_connectionPointOut;
}

AESConnectionPoint *AESConnectionLine::GetFirstFoundConnectedPoint() const
{
    if (GetConnectionPointIn())
    {
        return GetConnectionPointIn();
    }
    return GetConnectionPointOut();
}

bool AESConnectionLine::IsValidConnection(AESConnectionPoint *oneConnPoint,
                                          AESConnectionPoint *otherConnPoint) const
{
    if (oneConnPoint && otherConnPoint)
    {
        return (oneConnPoint->GetType() != otherConnPoint->GetType()) &&
               (oneConnPoint->GetSiblingConnectionPoint() != otherConnPoint);
    }
    return false;
}

Vector3 AESConnectionLine::GetConnectionPointLinePosition(
                                        AESConnectionPoint *connPoint) const
{
    Vector3 linePos = GetRectTransform()->FromWorldToLocalPoint(
                        connPoint->GetRectTransform()->FromLocalToWorldPoint(
                            Vector3(0, 0, 0)));
    linePos.z = -0.02f;
    return linePos;
}

