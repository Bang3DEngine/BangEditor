#include "BangEditor/AESNode.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/AESConnectionLine.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESNode::AESNode()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(-Vector2::One);
    rt->SetMarginRightTop( -Vector2i(180, 65) );

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetCursorType(Cursor::Type::HAND);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        menuRootItem->SetFontSize(12);
        MenuItem *createTransition = menuRootItem->AddItem("Create transition");
        createTransition->SetSelectedCallback([this](MenuItem*)
        {
            CreateTransition();
        });
    });
    p_contextMenu->SetFocusable(p_focusable);

    GameObject *panelGo = GameObjectFactory::CreateUIGameObject();
    p_bg = panelGo->AddComponent<UIImageRenderer>();
    panelGo->GetRectTransform()->SetLocalPosition( Vector3(0, 0, -0.0001f) );
    panelGo->AddComponent<UIRectMask>();
    p_border = GameObjectFactory::AddInnerBorder(panelGo);
    panelGo->SetParent(this);

    GameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    p_nameText = textContainer->AddComponent<UITextRenderer>();
    p_nameText->SetTextColor(Color::Black);
    p_nameText->SetContent("Animation State");
    textContainer->SetParent(panelGo);

    GetRectTransform()->SetLocalPosition( Vector3(0, 0, -0.0005f) );
}

AESNode::~AESNode()
{
}

void AESNode::Update()
{
    GameObject::Update();

    Color nodeColor = Color::White.WithValue(0.95f);
    RectTransform *rt = GetRectTransform();
    if ( p_focusable->IsBeingPressed() &&
        !Input::GetMouseButton(MouseButton::MIDDLE))
    {
        RectTransform *parentRT = GetParent()->GetRectTransform();
        Vector2 mouseDelta( Input::GetMouseDelta() );
        mouseDelta *= (1.0f / parentRT->GetLocalScale().x);

        Vector3 newLocalPos = rt->GetLocalPosition();
        newLocalPos += Vector3(mouseDelta, 0.0f);
        rt->SetLocalPosition(newLocalPos);
        nodeColor = UITheme::GetOverColor();
    }
    else
    {
        if (Input::GetMouseButtonDown(MouseButton::LEFT))
        {
            if (p_toConnectionLineBeingDragged &&
                m_framesPassedSinceLineDragStarted >= 1)
            {
                OnDragConnectionLineEnd();
            }
        }
        if (p_focusable->IsMouseOver())
        {
            nodeColor = UITheme::GetOverColor();
        }
    }
    p_bg->SetTint(nodeColor);
    ++m_framesPassedSinceLineDragStarted;
}

void AESNode::OnZoomScaleChanged(float zoomScale)
{
    p_nameText->SetTextSize(15 * zoomScale);
}

UIFocusable* AESNode::GetFocusable() const
{
    return p_focusable;
}

void AESNode::Connect(AESNode *fromNode, AESNode *toNode)
{
    ASSERT(false); // TODO
}

void AESNode::CreateTransition()
{
    if (p_toConnectionLineBeingDragged)
    {
        GameObject::Destroy(p_toConnectionLineBeingDragged);
        p_toConnectionLineBeingDragged = nullptr;
    }

    m_framesPassedSinceLineDragStarted = 0;
    p_toConnectionLineBeingDragged = GameObject::Create<AESConnectionLine>();
    p_toConnectionLineBeingDragged->SetNodeFrom(this);
    p_toConnectionLineBeingDragged->SetParent(this);
}

void AESNode::OnDragConnectionLineEnd()
{
    AESNode *nodeToConnectTo = nullptr;
    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        if (UIFocusable *overedFocus = canvas->GetFocusableUnderMouseTopMost())
        {
            GameObject *overedGo = overedFocus->GetGameObject();
            if (auto overedNode = DCAST<AESNode*>(overedGo))
            {
                if (this != overedNode)
                {
                    nodeToConnectTo = overedNode;
                }
            }
        }
    }

    if (nodeToConnectTo)
    {
        // Consolidate connection
        AESConnectionLine *newConnectionLine = p_toConnectionLineBeingDragged;
        newConnectionLine->SetNodeTo(nodeToConnectTo);
        p_toConnectionLines.PushBack(newConnectionLine);
        p_toConnectionLineToConnectedNode.Add(newConnectionLine, nodeToConnectTo);
        p_toConnectionLineBeingDragged = nullptr;
    }
    else
    {
        GameObject::Destroy(p_toConnectionLineBeingDragged); // Remove drag line
        p_toConnectionLineBeingDragged = nullptr;
    }
}

void AESNode::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (AESConnectionLine *connectionLine = DCAST<AESConnectionLine*>(object))
    {
        if (connectionLine == p_toConnectionLineBeingDragged)
        {
            p_toConnectionLineBeingDragged = nullptr;
        }
        p_toConnectionLines.Remove(connectionLine);
        p_toConnectionLineToConnectedNode.Remove(connectionLine);
    }
}

UIEventResult AESNode::OnUIEvent(UIFocusable *, const UIEvent &event)
{
    switch (event.type)
    {
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
