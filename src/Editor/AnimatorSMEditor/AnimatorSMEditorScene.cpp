#include "BangEditor/AnimatorSMEditorScene.h"

#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/UICanvas.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/AESNode.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/EditorTextureFactory.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

AnimatorSMEditorScene::AnimatorSMEditorScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIRectMask>();

    UIImageRenderer *nodesSceneBG = AddComponent<UIImageRenderer>();
    nodesSceneBG->SetTint(Color::DarkGray);

    GameObject *borderGo = GameObjectFactory::CreateUIGameObject();
    p_border = GameObjectFactory::AddInnerBorder(borderGo);
    borderGo->GetRectTransform()->SetLocalPosition( Vector3(0, 0, -0.04f) );
    borderGo->SetParent(this);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_mainContainer = GameObjectFactory::CreateUIGameObject();
    p_mainContainer->SetParent(this);

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem)
    {
        if (GetAnimatorSM() && !IsMouseOverSomeConnectionLine())
        {
            menuRootItem->SetFontSize(12);

            MenuItem *createNode = menuRootItem->AddItem("Create new node");
            createNode->SetSelectedCallback([this](MenuItem*)
            {
                GetAnimatorSM()->CreateAndAddNode();

                AESNode *aesNode = p_nodes[p_nodes.Size()-1];
                RectTransform *rt = aesNode->GetRectTransform();
                rt->SetAnchors(
                    aesNode->GetParent()->GetRectTransform()->FromWorldToLocalPoint(
                                Vector3(Input::GetMousePosition(), 0)).xy() );
            });
        }
    });
    p_contextMenu->SetFocusable(p_focusable);

    PropagateOnZoomScaleChanged();
}

AnimatorSMEditorScene::~AnimatorSMEditorScene()
{
}

void AnimatorSMEditorScene::Update()
{
    GameObject::Update();

    if (UICanvas::GetActive(this)->HasFocus(this, true))
    {
        if (Input::GetMouseButton(MouseButton::MIDDLE))
        {
            Vector2i mouseDelta = Input::GetMouseDelta();
            m_panning += mouseDelta;

            RectTransform *mainContRT = p_mainContainer->GetRectTransform();
            mainContRT->SetLocalPosition( Vector3(Vector2(m_panning), 0.0f) );
        }

        Vector2 mouseWheel = (Input::GetMouseWheel() * Vector2(0.015f));
        if (mouseWheel != Vector2::Zero)
        {
            m_zoomScale = Math::Clamp(m_zoomScale + mouseWheel.y, 0.1f, 2.0f);
            PropagateOnZoomScaleChanged();
        }
    }

    p_mainContainer->GetRectTransform()->SetLocalScale(
                Vector3(Vector2(m_zoomScale), 1.0f) );
}

void AnimatorSMEditorScene::CreateAndAddNode(AnimatorStateMachineNode *smNode,
                                             uint addIdx)
{
    AESNode *aesNode = GameObject::Create<AESNode>();
    aesNode->p_animatorSMEditorScene = this;
    aesNode->SetParent(p_mainContainer);

    smNode->EventEmitter<IEventsAnimatorStateMachineNode>::
            RegisterListener(aesNode);
    // smNode->EventEmitter<IEventsAnimatorStateMachineNode>::
    //          RegisterListener(this);

    ASSERT(addIdx <= p_nodes.Size());
    p_nodes.Insert(aesNode, addIdx);

    PropagateOnZoomScaleChanged();
}

void AnimatorSMEditorScene::SetAnimatorSM(AnimatorStateMachine *animatorSM)
{
    if (animatorSM != GetAnimatorSM())
    {
        Clear();
        p_animatorSM.Set(animatorSM);

        GetAnimatorSM()->EventEmitter<IEventsAnimatorStateMachine>::
                         RegisterListener(this);

        for (uint i = 0; i < GetAnimatorSM()->GetNodes().Size(); ++i)
        {
            ASSERT(i < GetAnimatorSM()->GetNodes().Size());

            AnimatorStateMachineNode *smNode = animatorSM->GetNode(i);
            ASSERT(smNode);

            OnNodeCreated(GetAnimatorSM(), i, smNode);
        }

        PropagateOnZoomScaleChanged();
    }
}

void AnimatorSMEditorScene::Clear()
{
    if (GetAnimatorSM())
    {
        GetAnimatorSM()->EventEmitter<IEventsAnimatorStateMachine>::
                         UnRegisterListener(this);
    }
    p_animatorSM.Set(nullptr);

    for (AESNode *node : p_nodes)
    {
        GameObject::Destroy(node);
    }
    p_nodes.Clear();
}

const Array<AESNode *> &AnimatorSMEditorScene::GetAESNodes() const
{
    return p_nodes;
}

AnimatorStateMachine *AnimatorSMEditorScene::GetAnimatorSM() const
{
    return p_animatorSM.Get();
}

void AnimatorSMEditorScene::PropagateOnZoomScaleChanged()
{
    for (AESNode *node : p_nodes)
    {
        node->OnZoomScaleChanged(m_zoomScale);
    }
}

bool AnimatorSMEditorScene::IsMouseOverSomeConnectionLine() const
{
    for (AESNode *aesNode : GetAESNodes())
    {
        for (AESConnectionLine *cl : aesNode->GetConnectionLines())
        {
            if (cl->IsMouseOver())
            {
                return true;
            }
        }
    }
    return false;
}

void AnimatorSMEditorScene::OnNodeCreated(AnimatorStateMachine *stateMachine,
                                          uint newNodeIdx,
                                          AnimatorStateMachineNode *newNode)
{
    ASSERT(stateMachine == GetAnimatorSM());
    CreateAndAddNode(newNode, newNodeIdx);
}

void AnimatorSMEditorScene::OnNodeRemoved(AnimatorStateMachine *stateMachine,
                                          uint removedNodeIdx,
                                          AnimatorStateMachineNode *removedNode)
{
    ASSERT(stateMachine == GetAnimatorSM());

    ASSERT(removedNodeIdx < p_nodes.Size());

    removedNode->EventEmitter<IEventsAnimatorStateMachineNode>::
                 UnRegisterListener(this);

    AESNode *aesNodeToRemove = p_nodes[removedNodeIdx];
    p_nodes.RemoveByIndex(removedNodeIdx);
    GameObject::Destroy(aesNodeToRemove);
}

void AnimatorSMEditorScene::
OnConnectionAdded(AnimatorStateMachineNode *node,
                  AnimatorStateMachineConnection *connection)
{

}

void AnimatorSMEditorScene::
OnConnectionRemoved(AnimatorStateMachineNode *node,
                    AnimatorStateMachineConnection *connection)
{

}

UIEventResult AnimatorSMEditorScene::OnUIEvent(UIFocusable *, const UIEvent &event)
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

