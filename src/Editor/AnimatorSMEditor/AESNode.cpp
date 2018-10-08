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

#include "BangEditor/Inspector.h"
#include "BangEditor/GIWAESNode.h"
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
    p_nodeNameText = textContainer->AddComponent<UITextRenderer>();
    p_nodeNameText->SetTextColor(Color::Black);
    SetNodeName("Animation State");
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
        SetNodeName(smNode->GetName());
    }

    Color nodeColor = Color::White.WithValue(0.95f);
    RectTransform *rt = GetRectTransform();
    if ( p_focusable->IsBeingPressed() &&
        !Input::GetMouseButton(MouseButton::MIDDLE))
    {
        float localPosZ = rt->GetLocalPosition().z;
        Vector3 newLocalPos =
             Vector3(p_aesScene->GetMousePositionInSceneSpace() - m_grabOffset,
                     localPosZ);
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

void AESNode::SetNodeName(const String &nodeName)
{
    if (nodeName != GetNodeName())
    {
        m_nodeName = nodeName;
        p_nodeNameText->SetContent( GetNodeName() );
    }
}

void AESNode::OnZoomScaleChanged(float zoomScale)
{
    p_nodeNameText->SetTextSize(18 * zoomScale);
}

const String &AESNode::GetNodeName() const
{
    return m_nodeName;
}

UIFocusable* AESNode::GetFocusable() const
{
    return p_focusable;
}

const Array<DPtr<AESConnectionLine>> &AESNode::GetConnectionLines() const
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

    // while (!GetConnectionLines().IsEmpty())
    // {
    //     AESConnectionLine *connLine = GetConnectionLines().Back();
    //     GameObject::DestroyImmediate(connLine);
    // }
    DestroyLineUsedForDragging();

    GetAnimatorSM()->RemoveNode(idx);
}

void AESNode::Duplicate()
{
    uint idx = GetIndexInStateMachine();
    ASSERT(idx != -1u);

    AnimatorStateMachineNode *newNode = GetAnimatorSM()->CreateAndAddNode();
    AESNode *newAESNode = p_aesScene->GetAESNodes().Back();
    ASSERT(newNode);

    AnimatorStateMachineNode *nodeToCloneFrom = GetSMNode();
    ASSERT(nodeToCloneFrom);

    nodeToCloneFrom->CloneInto(newNode);
    float localPosZ = newAESNode->GetRectTransform()->GetLocalPosition().z;
    newAESNode->GetRectTransform()->SetLocalPosition(
                Vector3(p_aesScene->GetMousePositionInSceneSpace(), localPosZ) );
    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        canvas->SetFocus( newAESNode->GetFocusable() );
    }
}

void AESNode::DestroyLineUsedForDragging()
{
    if (p_toConnectionLineBeingDragged)
    {
        GameObject::Destroy(p_toConnectionLineBeingDragged);
        p_toConnectionLineBeingDragged = nullptr;
    }
}

Vector2 AESNode::GetExportPosition() const
{
    return GetRectTransform()->GetLocalPosition().xy();
}

void AESNode::ImportPosition(const Vector2 &position)
{
    GetRectTransform()->SetLocalPosition(
                Vector3(position, GetRectTransform()->GetLocalPosition().z) );
}

uint AESNode::GetIndexInStateMachine() const
{
    return p_aesScene->GetAESNodes().IndexOf(
                const_cast<AESNode*>(this));
}

AnimatorStateMachine *AESNode::GetAnimatorSM() const
{
    return p_aesScene->GetAnimatorSM();
}

AnimatorStateMachineNode *AESNode::GetSMNode() const
{
    return GetAnimatorSM()->GetNode( GetIndexInStateMachine() );
}

void AESNode::OnConnectionAdded(const AnimatorStateMachineNode *node,
                                const AnimatorStateMachineConnection *connection)
{
    ASSERT( node == GetSMNode() );
    ASSERT( connection->GetNodeFromIndex() < p_aesScene->
                                             GetAESNodes().Size());
    ASSERT( connection->GetNodeToIndex() < p_aesScene->
                                           GetAESNodes().Size());

    AESConnectionLine *connLine = CreateAndAddDefinitiveConnection();
    AESNode *aesNodeFrom = p_aesScene->GetAESNodes()[
                                                connection->GetNodeFromIndex()];
    AESNode   *aesNodeTo = p_aesScene->GetAESNodes()[
                                                connection->GetNodeToIndex()];
    connLine->SetNodeFrom( aesNodeFrom );
    connLine->SetNodeTo( aesNodeTo );
}

void AESNode::OnConnectionRemoved(const AnimatorStateMachineNode *node,
                                  const AnimatorStateMachineConnection *connection)
{
    ASSERT(node == GetSMNode());

    uint indexOfSMConnection = connection->GetIndexInsideNodeConnections(
                                                GetIndexInStateMachine());

    if (indexOfSMConnection < GetConnectionLines().Size())
    {
        AESConnectionLine *connLine = GetConnectionLines()[indexOfSMConnection];
        GameObject::Destroy(connLine);
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
        {
            GameObjectFactory::MakeBorderFocused(p_border);
            if (Inspector *insp = Inspector::GetActive())
            {
                GIWAESNode *aesNodeInspWidget = GameObject::Create<GIWAESNode>();
                aesNodeInspWidget->SetAESNode(this);
                aesNodeInspWidget->Init();
                insp->ShowInspectorWidget(aesNodeInspWidget);
            }
            return UIEventResult::INTERCEPT;
        }
        break;

        case UIEvent::Type::MOUSE_CLICK_DOWN:
        if (event.mouse.button == MouseButton::LEFT)
        {
            m_grabOffset =
                p_aesScene->GetMousePositionInSceneSpace() -
                p_aesScene->GetWorldPositionInSceneSpace(
                    GetRectTransform()->GetViewportAARect().GetCenter() );
        }
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
