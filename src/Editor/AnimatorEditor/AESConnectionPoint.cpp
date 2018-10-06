#include "BangEditor/AESConnectionPoint.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/LineRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/TextureFactory.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESConnectionLine.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESConnectionPoint::AESConnectionPoint() : AESConnectionPoint(Type::IN)
{
}

AESConnectionPoint::AESConnectionPoint(Type type)
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    m_type = type;

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetCursorType(Cursor::Type::HAND);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_bg = AddComponent<UIImageRenderer>();
    p_bg->SetTint(Color::White);

    p_border = GameObjectFactory::AddInnerBorder(this);

    if (GetType() == Type::OUT)
    {
        GameObject *iconGo = GameObjectFactory::CreateUIGameObject();
        UIImageRenderer *icon = iconGo->AddComponent<UIImageRenderer>();
        icon->SetImageTexture( TextureFactory::GetRightArrowIcon() );
        icon->SetTint(Color::DarkGray);
        iconGo->GetRectTransform()->SetMargins(3);
        iconGo->SetParent(this);
    }
}

AESConnectionPoint::~AESConnectionPoint()
{
}

void AESConnectionPoint::Update()
{
    GameObject::Update();
}

void AESConnectionPoint::BeforeRender()
{
    GameObject::BeforeRender();

    // Doing this in BeforeRender, so that all layout is built
    Color nodeColor = Color::White;
    if (GetType() == Type::IN)
    {
        if (p_focusable->IsMouseOver())
        {
            UICanvas *canvas = UICanvas::GetActive(this);
            ASSERT(canvas);

            if (UIFocusable *focus = canvas->GetFocus())
            {
                if (AESConnectionPoint *focusedConnOut =
                        DCAST<AESConnectionPoint*>(focus->GetGameObject()))
                {
                    if (IsValidConnection(focusedConnOut))
                    {
                        nodeColor = Color::Green;
                    }
                }
            }
        }
    }
    else if (GetType() == Type::OUT)
    {
        if (p_focusable->IsBeingPressed())
        {
            nodeColor = UITheme::GetSelectedColor();

            if (!p_currentDragConnLine)
            {
                p_currentDragConnLine = GameObject::Create<AESConnectionLine>();
                p_currentDragConnLine->SetConnectionPointOut(this);
                p_currentDragConnLine->EventEmitter<IEventsDestroy>::
                                       RegisterListener(this);
                p_currentDragConnLine->SetParent(this);
            }

            UICanvas *canvas = UICanvas::GetActive(this);
            ASSERT(canvas);

            p_currentDragPointConnectingTo = nullptr;
            if (UIFocusable *overFocus = canvas->GetFocusableUnderMouseTopMost())
            {
                if (AESConnectionPoint *overedConnIn =
                        DCAST<AESConnectionPoint*>(overFocus->GetGameObject()))
                {
                    if (IsValidConnection(overedConnIn))
                    {
                        nodeColor = Color::Green;
                    }
                    p_currentDragPointConnectingTo = overedConnIn;
                }
            }
        }
        else
        {
            if (p_currentDragPointConnectingTo)
            {
                ASSERT(GetType() == Type::OUT);

                // Just dropped from a connection drag
                if (IsValidConnection(p_currentDragPointConnectingTo))
                {
                    ConnectTo(p_currentDragPointConnectingTo);
                    p_currentDragPointConnectingTo->ConnectTo(this);

                    p_currentDragConnLine->SetConnectionPointIn(
                                        p_currentDragPointConnectingTo);
                }
                p_currentDragPointConnectingTo = nullptr;
            }
            else
            {
                if (p_focusable->IsMouseOver())
                {
                    nodeColor = UITheme::GetOverColor();
                }
            }
        }
    }
    p_bg->SetTint(nodeColor);
}

void AESConnectionPoint::SetNode(AESNode *node)
{
    p_node = node;
}

void AESConnectionPoint::SetConnectionLine(AESConnectionLine *connectionLine)
{
    if (connectionLine != GetConnectionLine())
    {
        if (GetConnectionLine())
        {
            GameObject::Destroy(p_connectionLine);
        }

        p_connectionLine = connectionLine;
    }
}

void AESConnectionPoint::SetSiblingConnectionPoint(
                                AESConnectionPoint *siblingConnectionPoint)
{
    ASSERT( siblingConnectionPoint->GetType() != GetType() );
    p_siblingConnectionPoint = siblingConnectionPoint;
}

void AESConnectionPoint::ConnectTo(AESConnectionPoint *toConnectionPoint)
{
    p_pointConnectedTo = toConnectionPoint;
}

AESNode *AESConnectionPoint::GetNode() const
{
    return p_node;
}

UIFocusable *AESConnectionPoint::GetFocusable() const
{
    return p_focusable;
}

AESConnectionPoint::Type AESConnectionPoint::GetType() const
{
    return m_type;
}

AESConnectionPoint *AESConnectionPoint::GetConnection() const
{
    return p_pointConnectedTo;
}

AESConnectionLine *AESConnectionPoint::GetConnectionLine() const
{
    return p_connectionLine;
}

AESConnectionPoint *AESConnectionPoint::GetSiblingConnectionPoint() const
{
    return p_siblingConnectionPoint;
}

bool AESConnectionPoint::IsValidConnection(AESConnectionPoint *otherConnPoint) const
{
    return (otherConnPoint->GetType() != GetType()) &&
            (otherConnPoint != GetSiblingConnectionPoint());
}

void AESConnectionPoint::Connect(AESConnectionPoint *oneConnPoint,
                                 AESConnectionPoint *otherConnPoint)
{
    ASSERT(oneConnPoint);
    ASSERT(otherConnPoint);
    ASSERT(oneConnPoint->IsValidConnection(otherConnPoint));
    oneConnPoint->ConnectTo(otherConnPoint);
    otherConnPoint->ConnectTo(oneConnPoint);
}

void AESConnectionPoint::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (object == p_currentDragConnLine)
    {
        p_currentDragConnLine = nullptr;
    }
}

UIEventResult AESConnectionPoint::OnUIEvent(UIFocusable *,
                                            const UIEvent &event)
{
    switch (event.type)
    {
        case UIEvent::Type::STARTED_BEING_PRESSED:
        case UIEvent::Type::MOUSE_CLICK_DOWN:
        case UIEvent::Type::MOUSE_CLICK_UP:
        case UIEvent::Type::MOUSE_ENTER:
        case UIEvent::Type::MOUSE_EXIT:
            return UIEventResult::INTERCEPT;
        break;

        case UIEvent::Type::FOCUS_LOST:
            // GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        case UIEvent::Type::FOCUS_TAKEN:
            // GameObjectFactory::MakeBorderFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

