#include "BangEditor/AESNode.h"

#include "Bang/Alignment.h"
#include "Bang/Animator.h"
#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineBlendTreeNode.h"
#include "Bang/AnimatorStateMachineLayer.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/AnimatorStateMachinePlayer.h"
#include "Bang/AnimatorStateMachineTransition.h"
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
#include "Bang/Vector2.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/AnimatorSMEditorScene.h"
#include "BangEditor/Editor.h"
#include "BangEditor/GIWAESNode.h"
#include "BangEditor/GIWAESNodeBlendTree.h"
#include "BangEditor/Inspector.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

using namespace Bang;
using namespace BangEditor;

#include "Bang/Debug.h"
AESNode::AESNode()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    RectTransform *rt = GetRectTransform();
    rt->SetAnchors(-Vector2::One());
    rt->SetPivotPosition(Vector2::Zero());
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
    p_nodeNameText->SetTextColor(Color::Black());
    nameTextContainer->SetParent(panelGo);

    GameObject *entryNodeTextContainer =
        GameObjectFactory::CreateUIGameObject();
    p_entryNodeText = entryNodeTextContainer->AddComponent<UITextRenderer>();
    p_entryNodeText->SetTextColor(Color::Black());
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

    Color nodeColor = Color::White().WithValue(0.95f);
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
        p_entryNodeText->SetEnabled(GetAnimatorSMLayer()->GetEntryNode() ==
                                    smNode);
        if (Animator *animator = GetCurrentAnimator())
        {
            for (AnimatorStateMachinePlayer *player : animator->GetPlayers())
            {
                if (player->GetCurrentNode() == smNode)
                {
                    nodeColor = Color::Green();
                }
            }
        }
    }

    p_bg->SetTint(nodeColor);
    ++m_framesPassedSinceLineDragStarted;
}

void AESNode::SetAsEntryNode()
{
    MetaNode previousMeta = GetAnimatorSM()->GetMeta();

    GetAnimatorSMLayer()->SetEntryNode(GetSMNode());

    UndoRedoManager::PushAction(new UndoRedoSerializableChange(
        GetAnimatorSM(), previousMeta, GetAnimatorSM()->GetMeta()));
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
    p_nodeNameText->SetTextSize(SCAST<int>(18 * zoomScale));
    p_entryNodeText->SetTextSize(SCAST<int>(12 * zoomScale));
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
    p_toConnectionLineBeingDragged = new AESConnectionLine();
    p_toConnectionLineBeingDragged->SetNodeFrom(this);
    p_toConnectionLineBeingDragged->SetParent(this);
}

AESConnectionLine *AESNode::CreateAndAddDefinitiveConnection()
{
    AESConnectionLine *connLine = new AESConnectionLine();
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

        MetaNode previousMeta = GetAnimatorSM()->GetMeta();

        AnimatorStateMachineNode *fromSMNode = GetSMNode();
        AnimatorStateMachineNode *toSMNode = nodeToConnectTo->GetSMNode();
        ASSERT(fromSMNode);
        ASSERT(toSMNode);
        fromSMNode->CreateTransitionTo(toSMNode);

        UndoRedoManager::PushAction(new UndoRedoSerializableChange(
            GetAnimatorSM(), previousMeta, GetAnimatorSM()->GetMeta()));
    }
    else
    {
        DestroyLineUsedForDragging();
    }
}

void AESNode::RemoveSelf()
{
    MetaNode previousMeta = GetAnimatorSM()->GetMeta();

    uint idx = GetIndexInStateMachine();
    if (idx != SCAST<uint>(-1))
    {
        DestroyLineUsedForDragging();
        GetAnimatorSMLayer()->RemoveNode(GetAnimatorSMLayer()->GetNodes()[idx]);
    }

    UndoRedoManager::PushAction(new UndoRedoSerializableChange(
        GetAnimatorSM(), previousMeta, GetAnimatorSM()->GetMeta()));
}

void AESNode::Duplicate()
{
    MetaNode previousMeta = GetAnimatorSM()->GetMeta();

    uint idx = GetIndexInStateMachine();
    ASSERT(idx != SCAST<uint>(-1));

    AnimatorStateMachineNode *nodeToCloneFrom = GetSMNode();
    ASSERT(nodeToCloneFrom);

    AnimatorStateMachineNode *newNode =
        SCAST<AnimatorStateMachineNode *>(nodeToCloneFrom->Clone(false));
    GetAnimatorSMLayer()->AddNode(newNode);
    AESNode *newAESNode = GetAESScene()->GetAESNodes().Back();
    ASSERT(newNode);

    nodeToCloneFrom->CloneInto(newNode, false);
    float localPosZ = newAESNode->GetRectTransform()->GetLocalPosition().z;
    newAESNode->GetRectTransform()->SetLocalPosition(
        Vector3(GetAESScene()->GetMousePositionInSceneSpace(), localPosZ));
    if (UICanvas *canvas = UICanvas::GetActive(this))
    {
        canvas->SetFocus(newAESNode->GetFocusable());
    }

    UndoRedoManager::PushAction(new UndoRedoSerializableChange(
        GetAnimatorSM(), previousMeta, GetAnimatorSM()->GetMeta()));
}

void AESNode::DestroyLineUsedForDragging()
{
    if (p_toConnectionLineBeingDragged)
    {
        GameObject::Destroy(p_toConnectionLineBeingDragged);
        p_toConnectionLineBeingDragged = nullptr;
    }
}

void AESNode::SetSMNode(AnimatorStateMachineNode *smNode)
{
    ASSERT(!p_smNode);
    p_smNode = smNode;
    p_smNode->EventEmitter<IEventsDestroy>::RegisterListener(this);
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

AnimatorStateMachineLayer *AESNode::GetAnimatorSMLayer() const
{
    return GetSMNode() ? GetSMNode()->GetLayer() : nullptr;
}

AnimatorStateMachineNode *AESNode::GetSMNode() const
{
    return p_smNode;
}

void AESNode::OnTransitionAdded(AnimatorStateMachineNode *node,
                                AnimatorStateMachineTransition *transition)
{
    ASSERT(node == GetSMNode());
    ASSERT(transition->GetNodeTo());
    ASSERT(transition->GetNodeFrom());

    AnimatorStateMachineNode *nodeTo = transition->GetNodeTo();
    ASSERT(nodeTo);
    if (!p_nodeConnectedToToConnectionLine.ContainsKey(nodeTo))
    {
        AnimatorStateMachineLayer *smLayer = GetAnimatorSMLayer();
        uint nodeFromIdx =
            smLayer->GetNodes().IndexOf(transition->GetNodeFrom());
        uint nodeToIdx = smLayer->GetNodes().IndexOf(transition->GetNodeTo());
        AESConnectionLine *connLine = CreateAndAddDefinitiveConnection();
        AESNode *aesNodeFrom = GetAESScene()->GetAESNodes()[nodeFromIdx];
        AESNode *aesNodeTo = GetAESScene()->GetAESNodes()[nodeToIdx];
        connLine->SetNodeFrom(aesNodeFrom);
        connLine->SetNodeTo(aesNodeTo);
        p_nodeConnectedToToConnectionLine.Add(nodeTo, connLine);
    }
}

void AESNode::OnTransitionRemoved(
    AnimatorStateMachineNode *node,
    AnimatorStateMachineTransition *transitionToRemove)
{
    AnimatorStateMachineNode *smNode = GetSMNode();
    ASSERT(node == smNode);

    AnimatorStateMachineNode *nodeTo = transitionToRemove->GetNodeTo();
    ASSERT(nodeTo);
    if (p_nodeConnectedToToConnectionLine.ContainsKey(nodeTo))
    {
        bool theresStillSomeConnectionToNodeTo = false;
        for (AnimatorStateMachineTransition *conn : node->GetTransitions())
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
                GIWAESNode *aesNodeInspWidget = nullptr;
                if (DCAST<AnimatorStateMachineBlendTreeNode *>(GetSMNode()))
                {
                    aesNodeInspWidget = new GIWAESNodeBlendTree();
                }
                else
                {
                    aesNodeInspWidget = new GIWAESNode();
                }
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

void AESNode::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    if (object == GetSMNode())
    {
        GameObject::Destroy(this);
        Object::PropagateObjectDestruction(this);
    }
}
