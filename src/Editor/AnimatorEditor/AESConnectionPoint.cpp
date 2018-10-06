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
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "Bang/TextureFactory.h"

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
        p_connectionLR = AddComponent<LineRenderer>();
        p_connectionLR->SetViewProjMode(GL::ViewProjMode::CANVAS);
        p_connectionLR->GetMaterial()->SetAlbedoColor(Color::Green);
        p_connectionLR->GetMaterial()->SetRenderPass(RenderPass::CANVAS);
        p_connectionLR->GetMaterial()->SetLineWidth(5.0f);
        p_connectionLR->SetPoints({Vector3::Zero, Vector3::Zero});


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
        Color lineColor = Color::Red;
        if (p_focusable->IsBeingPressed())
        {
            Vector2i mousePosition = Input::GetMousePosition();
            Vector2 linePoint = GetRectTransform()->FromWorldToLocalPoint(
                                            Vector3(mousePosition, 0)).xy();
            p_connectionLR->SetPoint(1, Vector3(linePoint, -0.01f));
            nodeColor = UITheme::GetSelectedColor();

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
                        lineColor = Color::Green;
                    }
                    else
                    {
                        lineColor = Color::Red;
                    }
                    p_currentDragPointConnectingTo = overedConnIn;
                }
            }
        }
        else
        {
            if (p_currentDragPointConnectingTo)
            {
                // Just dropped from a connection drag
                if (IsValidConnection(p_currentDragPointConnectingTo))
                {
                    ConnectTo(p_currentDragPointConnectingTo);
                    p_currentDragPointConnectingTo->ConnectTo(this);
                }
                p_currentDragPointConnectingTo = nullptr;
            }
            else
            {
                if (GetConnection() && GetType() == Type::OUT)
                {
                    RectTransform *thisRT = GetRectTransform();
                    RectTransform *connRT = GetConnection()->GetRectTransform();
                    Vector3 connLocalPos = Vector3(Vector2::Zero, -0.01f);
                    p_connectionLR->SetPoint(1, thisRT->FromWorldToLocalPoint(
                                                 connRT->FromLocalToWorldPoint(
                                                                connLocalPos)));
                }
                else
                {
                    p_connectionLR->SetPoint(1, Vector3::Zero);
                }

                if (p_focusable->IsMouseOver())
                {
                    nodeColor = UITheme::GetOverColor();
                }
            }

            if (GetConnection())
            {
                lineColor = Color::Green;
            }
        }
        p_connectionLR->GetMaterial()->SetAlbedoColor(lineColor);
    }
    p_bg->SetTint(nodeColor);
}

void AESConnectionPoint::SetNode(AESNode *node)
{
    p_node = node;
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

AESConnectionPoint::Type AESConnectionPoint::GetType() const
{
    return m_type;
}

AESConnectionPoint *AESConnectionPoint::GetConnection() const
{
    return p_pointConnectedTo;
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
            GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        case UIEvent::Type::FOCUS_TAKEN:
            GameObjectFactory::MakeBorderFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

