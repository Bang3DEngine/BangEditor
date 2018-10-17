#include "BangEditor/AESNode.h"

#include "Bang/Alignment.h"
#include "Bang/Animator.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineConnection.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachinePlayer.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/Cursor.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsAnimatorStateMachineNode.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/Map.tcc"
#include "Bang/MouseButton.h"
#include "Bang/RectTransform.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIRectMask.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UITheme.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/AnimatorSMEditorScene.h"
#include "BangEditor/Editor.h"
#include "BangEditor/GIWAESNode.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"

using namespace Bang;
using namespace BangEditor;

AESNode::AESNode()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(-Vector2::One);
    rt->SetPivotPosition(Vector2::Zero);
    rt->SetSizeFromPivot(Vector2i(180, 65));

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetCursorType(Cursor::Type::HAND);
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem) {
        menuRootItem->SetFontSize(12);

        MenuItem *createTransition = menuRootItem->AddItem("Create transition");
        createTransition->SetSelectedCallback(
            [this](MenuItem *) { CreateAndAddConnectionToBeginDrag(); });

        MenuItem *duplicateNode = menuRootItem->AddItem("Duplicate");
        duplicateNode->SetSelectedCallback([this](MenuItem *) { Duplicate(); });

        MenuItem *removeNode = menuRootItem->AddItem("Remove node");
        removeNode->SetSelectedCallback([this](MenuItem *) { RemoveSelf(); });

        menuRootItem->AddSeparator();

        MenuItem *setAsEntry = menuRootItem->AddItem("Set as entry");
        setAsEntry->SetSelectedCallback(
            [this](MenuItem *) { SetAsEntryNode(); });

    });
    p_contextMenu->SetFocusable(p_focusable);

    GameObject *panelGo = GameObjectFactory::CreateUIGameObject();
    p_bg = panelGo->AddComponent<UIImageRenderer>();
    panelGo->GetRectTransform()->SetLocalPosition(Vector3(0, 0, -0.0001f));
    panelGo->AddComponent<UIRectMask>();
    p_border = GameObjectFactory::AddInnerBorder(panelGo);
    panelGo->SetParent(this);

    GameObject *nameTextContainer = GameObjectFactory::CreateUIGameObject();
    p_nodeNameText = nameTextContainer->AddComponent<UITextRenderer>();
    p_nodeNameText->SetTextColor(Color::Black);
    nameTextContainer->SetParent(panelGo);

    GameObject *entryNodeTextContainer =
        GameObjectFactory::CreateUIGameObject();
    p_entryNodeText = entryNodeTextContainer->AddComponent<UITextRenderer>();
    p_entryNodeText->SetTextColor(Color::Black);
    p_entryNodeText->SetContent("-Entry-");
    p_entryNodeText->SetHorizontalAlign(HorizontalAlignment::LEFT);
    p_entryNodeText->SetVerticalAlign(VerticalAlignment::TOP);
    entryNodeTextContainer->GetRectTransform()->SetMarginTop(5);
    entryNodeTextContainer->GetRectTransform()->SetMarginLeft(10);
    entryNodeTextContainer->SetParent(panelGo);

    GetRectTransform()->SetLocalPosition(Vector3(0, 0, -0.0005f));
    SetNodeName("");
}

AESNode::~AESNode()
{
}

void AESNode::Update()
{
    GameObject::Update();

    Color nodeColor = Color::White.WithValue(0.95f);
    RectTransform *rt = GetRectTransform();
    if (p_focusable->IsBeingPressed() &&
        !Input::GetMouseButton(MouseButton::MIDDLE))
    {
        float localPosZ = rt->GetLocalPosition().z;
        Vector3 newLocalPos = Vector3(
            GetAESScene()->GetMousePositionInSceneSpace() - m_grabOffset,
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

    if (AnimatorStateMachineNode *smNode = GetSMNode())
    {
        SetNodeName(smNode->GetName());
        p_entryNodeText->SetEnabled(GetAnimatorSM()->GetEntryNode() == smNode);
        if (Animator *animator = GetCurrentAnimator())
        {
            if (AnimatorStateMachinePlayer *player = animator->GetPlayer())
            {
                if (player->GetCurrentNode() == smNode)
                {
                    nodeColor = Color::Green;
                }
            }
        }
    }

    p_bg->SetTint(nodeColor);
    ++m_framesPassedSinceLineDragStarted;
}

void AESNode::SetAsEntryNode()
{
    GetAnimatorSM()->SetEntryNode(GetSMNode());
}

void AESNode::SetNodeName(const String &nodeName)
{
    if (nodeName != GetNodeName())
    {
        m_nodeName = nodeName;
        p_nodeNameText->SetContent(GetNodeName());
    }
}

void AESNode::OnZoomScaleChanged(float zoomScale)
{
    p_nodeNameText->SetTextSize(18 * zoomScale);
    p_entryNodeText->SetTextSize(12 * zoomScale);
}

const String &AESNode::GetNodeName() const
{
    return m_nodeName;
}

UIFocusable *AESNode::GetFocusable() const
{
    return p_focusable;
}

const Array<AESConnectionLine *> &AESNode::GetConnectionLines() const
{
    return p_connectionLinesTo;
}

Animator *AESNode::GetCurrentAnimator() const
{
    GameObject *selectedGameObject =
        Editor::GetInstance()->GetSelectedGameObject();
    return selectedGameObject ? selectedGameObject->GetComponent<Animator>()
                              : nullptr;
}

AnimatorSMEditorScene *AESNode::GetAESScene() const
{
    return p_aesScene;
}

void AESNode::CreateAndAddConnectionToBeginDrag()
{
    DestroyLineUsedForDragging();
    m_framesPassedSinceLineDragStarted = 0;
    p_toConnectionLineBeingDragged = GameObject::Create<AESConnectionLine>();
    p_toConnectionLineBeingDragged->SetNodeFrom(this);
    p_toConnectionLineBeingDragged->SetParent(this);
}

AESConnectionLine *AESNode::CreateAndAddDefinitiveConnection()
{
    AESConnectionLine *connLine = GameObject::Create<AESConnectionLine>();
    connLine->SetNodeFrom(this);
    connLine->SetParent(this);
    p_connectionLinesTo.PushBack(connLine);
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
            if (auto overedNode = DCAST<AESNode *>(overedGo))
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

        AnimatorStateMachineNode *fromSMNode = GetSMNode();
        AnimatorStateMachineNode *toSMNode = nodeToConnectTo->GetSMNode();
        ASSERT(fromSMNode);
        ASSERT(toSMNode);
        fromSMNode->CreateConnectionTo(toSMNode);
    }
    else
    {
        DestroyLineUsedForDragging();
    }
}

void AESNode::RemoveSelf()
{
    uint idx = GetIndexInStateMachine();
    if (idx != -1u)
    {
        DestroyLineUsedForDragging();
        GetAnimatorSM()->RemoveNode(GetAnimatorSM()->GetNodes()[idx]);
    }
}

void AESNode::Duplicate()
{
    uint idx = GetIndexInStateMachine();
    ASSERT(idx != -1u);

    AnimatorStateMachineNode *newNode = GetAnimatorSM()->CreateAndAddNode();
    AESNode *newAESNode = GetAESScene()->GetAESNodes().Back();
    ASSERT(newNode);

    AnimatorStateMachineNode *nodeToCloneFrom = GetSMNode();
    ASSERT(nodeToCloneFrom);

    nodeToCloneFrom->CloneInto(newNode);
    float localPosZ = newAESNode->GetRectTransform()->GetLocalPosition().z;
    newAESNode->GetRectTransform()->SetLocalPosition(
        Vector3(GetAESScene()->GetMousePositionInSceneSpace(), localPosZ));
    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        canvas->SetFocus(newAESNode->GetFocusable());
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
        Vector3(position, GetRectTransform()->GetLocalPosition().z));
}

uint AESNode::GetIndexInStateMachine() const
{
    return GetAESScene()->GetAESNodes().IndexOf(const_cast<AESNode *>(this));
}

AnimatorStateMachine *AESNode::GetAnimatorSM() const
{
    return GetAESScene()->GetAnimatorSM();
}

AnimatorStateMachineNode *AESNode::GetSMNode() const
{
    return GetAnimatorSM() ? GetAnimatorSM()->GetNode(GetIndexInStateMachine())
                           : nullptr;
}

void AESNode::OnConnectionAdded(AnimatorStateMachineNode *node,
                                AnimatorStateMachineConnection *connection)
{
    ASSERT(node == GetSMNode());
    ASSERT(connection->GetNodeTo());
    ASSERT(connection->GetNodeFrom());

    AnimatorStateMachineNode *nodeTo = connection->GetNodeTo();
    ASSERT(nodeTo);
    if (!p_nodeConnectedToToConnectionLine.ContainsKey(nodeTo))
    {
        AnimatorStateMachine *sm = GetAnimatorSM();
        uint nodeFromIdx = sm->GetNodes().IndexOf(connection->GetNodeFrom());
        uint nodeToIdx = sm->GetNodes().IndexOf(connection->GetNodeTo());
        AESConnectionLine *connLine = CreateAndAddDefinitiveConnection();
        AESNode *aesNodeFrom = GetAESScene()->GetAESNodes()[nodeFromIdx];
        AESNode *aesNodeTo = GetAESScene()->GetAESNodes()[nodeToIdx];
        connLine->SetNodeFrom(aesNodeFrom);
        connLine->SetNodeTo(aesNodeTo);
        p_nodeConnectedToToConnectionLine.Add(nodeTo, connLine);
    }
}

void AESNode::OnConnectionRemoved(AnimatorStateMachineNode *node,
                                  AnimatorStateMachineConnection *connToRemove)
{
    if (!GetAnimatorSM())
    {
        return;
    }

    ASSERT(node == GetSMNode());

    AnimatorStateMachineNode *nodeTo = connToRemove->GetNodeTo();
    ASSERT(nodeTo);
    if (p_nodeConnectedToToConnectionLine.ContainsKey(nodeTo))
    {
        bool theresStillSomeConnectionToNodeTo = false;
        for (AnimatorStateMachineConnection *conn : node->GetConnections())
        {
            if (conn->GetNodeTo() == nodeTo)
            {
                theresStillSomeConnectionToNodeTo = true;
                break;
            }
        }

        if (!theresStillSomeConnectionToNodeTo)
        {
            AESConnectionLine *connLine =
                p_nodeConnectedToToConnectionLine.Get(nodeTo);
            GameObject::Destroy(connLine);
            p_connectionLinesTo.Remove(connLine);
            p_nodeConnectedToToConnectionLine.Remove(nodeTo);
        }
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
                GIWAESNode *aesNodeInspWidget =
                    GameObject::Create<GIWAESNode>();
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
                    GetAESScene()->GetMousePositionInSceneSpace() -
                    GetAESScene()->GetWorldPositionInSceneSpace(
                        GetRectTransform()->GetViewportAARect().GetCenter());
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

                case Key::DELETE: RemoveSelf(); break;

                default: break;
            }
            GameObjectFactory::MakeBorderNotFocused(p_border);
            return UIEventResult::INTERCEPT;
            break;

        default: break;
    }
    return UIEventResult::IGNORE;
}
