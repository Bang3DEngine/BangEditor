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
#include "Bang/AnimatorStateMachineNode.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/AnimatorSMEditorScene.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AESNode::AESNode()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(-Vector2::One);
    rt->SetPivotPosition( Vector2::Zero );
    rt->SetSizeFromPivot( Vector2i(180, 65) );

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
            CreateAndAddConnectionToBeginDrag();
        });

        MenuItem *duplicateNode = menuRootItem->AddItem("Duplicate");
        duplicateNode->SetSelectedCallback([this](MenuItem*)
        {
            Duplicate();
        });

        MenuItem *removeNode = menuRootItem->AddItem("Remove node");
        removeNode->SetSelectedCallback([this](MenuItem*)
        {
            RemoveSelf();
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

    if (AnimatorStateMachineNode *smNode = GetSMNode())
    {
        p_nameText->SetContent(smNode->GetName());
    }

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
    p_nameText->SetTextSize(18 * zoomScale);
}

UIFocusable* AESNode::GetFocusable() const
{
    return p_focusable;
}

const Array<AESConnectionLine *> &AESNode::GetConnectionLines() const
{
    return p_toConnectionLines;
}

void AESNode::CreateAndAddConnectionToBeginDrag()
{
    DestroyLineUsedForDragging();
    m_framesPassedSinceLineDragStarted = 0;
    p_toConnectionLineBeingDragged = GameObject::Create<AESConnectionLine>();
    p_toConnectionLineBeingDragged->SetNodeFrom(this);
    p_toConnectionLineBeingDragged->SetParent(this);
}

AESConnectionLine* AESNode::CreateAndAddDefinitiveConnection()
{
    AESConnectionLine *connLine = GameObject::Create<AESConnectionLine>();
    connLine->SetNodeFrom(this);
    connLine->SetParent(this);
    return connLine;
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
        DestroyLineUsedForDragging();

        AESConnectionLine *newConnectionLine = CreateAndAddDefinitiveConnection();
        newConnectionLine->SetNodeTo(nodeToConnectTo);
        p_toConnectionLines.PushBack(newConnectionLine);
        p_toConnectionLineToConnectedNode.Add(newConnectionLine,
                                              nodeToConnectTo);
        nodeToConnectTo->EventEmitter<IEventsDestroy>::RegisterListener(this);

        AnimatorStateMachineNode *fromNode = GetSMNode();
        ASSERT(fromNode);
        AnimatorStateMachineNode *toNode = nodeToConnectTo->GetSMNode();
        ASSERT(toNode);
        fromNode->CreateConnectionTo(toNode);
    }
    else
    {
        DestroyLineUsedForDragging();
    }
}

void AESNode::RemoveSelf()
{
    uint idx = GetIndexInStateMachine();
    ASSERT(idx != -1u);

    GetAnimatorSM()->RemoveNode(idx);
}

void AESNode::Duplicate()
{
    uint idx = GetIndexInStateMachine();
    ASSERT(idx != -1u);

    AnimatorStateMachineNode *newNode = GetAnimatorSM()->CreateAndAddNode();
    ASSERT(newNode);

    AnimatorStateMachineNode *nodeToCloneFrom = GetSMNode();
    ASSERT(nodeToCloneFrom);

    nodeToCloneFrom->CloneInto(newNode);
}

void AESNode::DestroyLineUsedForDragging()
{
    if (p_toConnectionLineBeingDragged)
    {
        GameObject::Destroy(p_toConnectionLineBeingDragged);
        p_toConnectionLineBeingDragged = nullptr;
    }
}

uint AESNode::GetIndexInStateMachine() const
{
    return p_animatorSMEditorScene->GetAESNodes().IndexOf(
                const_cast<AESNode*>(this));
}

AnimatorStateMachine *AESNode::GetAnimatorSM() const
{
    return p_animatorSMEditorScene->GetAnimatorSM();
}

AnimatorStateMachineNode *AESNode::GetSMNode() const
{
    return GetAnimatorSM()->GetNode( GetIndexInStateMachine() );
}

void AESNode::OnConnectionAdded(AnimatorStateMachineNode *node,
                                AnimatorStateMachineConnection *connection)
{
    ASSERT( node == GetSMNode() );
    ASSERT( connection->GetNodeFromIndex() < p_animatorSMEditorScene->
                                             GetAESNodes().Size());
    ASSERT( connection->GetNodeToIndex() < p_animatorSMEditorScene->
                                           GetAESNodes().Size());

    AESConnectionLine *connLine = CreateAndAddDefinitiveConnection();
    AESNode *aesNodeFrom = p_animatorSMEditorScene->GetAESNodes()[
                                                connection->GetNodeFromIndex()];
    AESNode   *aesNodeTo = p_animatorSMEditorScene->GetAESNodes()[
                                                connection->GetNodeToIndex()];
    connLine->SetNodeFrom( aesNodeFrom );
    connLine->SetNodeTo( aesNodeTo );
}

void AESNode::OnConnectionRemoved(AnimatorStateMachineNode *node,
                                  AnimatorStateMachineConnection *connection)
{
    ASSERT(node == GetSMNode());

    uint indexOfSMConnection = connection->GetIndexInsideNodeConnections(
                                                GetIndexInStateMachine());
    ASSERT(indexOfSMConnection < GetConnectionLines().Size());

    AESConnectionLine *connLine = GetConnectionLines()[indexOfSMConnection];
    GameObject::Destroy(connLine);
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
    else if (AESNode *node = DCAST<AESNode*>(object))
    {
        p_toConnectedNodes.RemoveAll(node);
        for (auto it = p_toConnectionLines.Begin();
             it != p_toConnectionLines.End();)
        {
            AESConnectionLine *connectedLine = *it;
            if (connectedLine->GetNodeFrom() == node ||
                connectedLine->GetNodeTo() == node)
            {
                it = p_toConnectionLines.Remove(it);
                GameObject::Destroy(connectedLine);
            }
            else
            {
                ++it;
            }
        }
        node->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
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

        case UIEvent::Type::KEY_DOWN:
            switch (event.key.key)
            {
                case Key::D:
                if (event.key.modifiers.IsOn(KeyModifier::LCTRL))
                {
                    Duplicate();
                }
                break;

                case Key::DELETE:
                    RemoveSelf();
                break;

                default:
                break;
            }
            GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
        break;


        default:
        break;
    }
    return UIEventResult::IGNORE;
}
