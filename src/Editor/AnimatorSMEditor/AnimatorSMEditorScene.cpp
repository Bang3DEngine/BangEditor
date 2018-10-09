#include "BangEditor/AnimatorSMEditorScene.h"

#include "Bang/File.h"
#include "Bang/Input.h"
#include "Bang/UITheme.h"
#include "Bang/MetaNode.h"
#include "Bang/UICanvas.h"
#include "Bang/Material.h"
#include "Bang/UIRectMask.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/TextureFactory.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/MetaFilesManager.h"
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
    p_mainContainer->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_mainContainer->SetParent(this);

    p_gridContainer = GameObjectFactory::CreateUIGameObject();
    p_gridContainer->GetRectTransform()->SetAnchors( Vector2::Zero );
    p_gridContainer->GetRectTransform()->SetPivotPosition( Vector2::Zero );
    p_gridContainer->GetRectTransform()->SetSizeFromPivot( Vector2i(100000) );
    p_gridContainer->SetParent(this);

    p_gridImg = p_gridContainer->AddComponent<UIImageRenderer>();
    p_gridImg->SetImageTexture( EditorTextureFactory::GetGrid2x2() );
    p_gridImg->GetMaterial()->SetAlbedoUvMultiply( Vector2(10000.0f) );

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
                RectTransform *nodeRT = aesNode->GetRectTransform();
                float localPosZ = nodeRT->GetLocalPosition().z;
                Vector3 localPos = Vector3(GetMousePositionInSceneSpace(),
                                           localPosZ);
                nodeRT->SetLocalPosition(localPos);
            });
        }
    });
    p_contextMenu->SetFocusable(p_focusable);

    UpdatePanningAndZoomOnTransforms();
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
            Vector2 mouseDelta( Input::GetMouseDelta() );
            m_panning += mouseDelta;
        }

        Vector2 mouseWheel = (Input::GetMouseWheel() * Vector2(0.05f));
        if (mouseWheel != Vector2::Zero)
        {
            SetZoomScale( m_zoomScale + mouseWheel.y );
        }
    }

    UpdatePanningAndZoomOnTransforms();
}

void AnimatorSMEditorScene::CreateAndAddNode(AnimatorStateMachineNode *smNode,
                                             uint addIdx)
{
    AESNode *aesNode = GameObject::Create<AESNode>();
    aesNode->p_aesScene = this;
    aesNode->SetParent(p_mainContainer);

    smNode->EventEmitter<IEventsAnimatorStateMachineNode>::
            RegisterListener(aesNode);

    ASSERT(addIdx <= p_nodes.Size());
    p_nodes.Insert(aesNode, addIdx);

    UpdatePanningAndZoomOnTransforms();
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

        for (uint i = 0; i < GetAnimatorSM()->GetNodes().Size(); ++i)
        {
            AESNode *aesNode = GetAESNodes()[i];
            AnimatorStateMachineNode *smNode = GetAnimatorSM()->GetNode(i);
            for (AnimatorStateMachineConnection *conn :
                 GetAnimatorSM()->GetNode(i)->GetConnections())
            {
                aesNode->OnConnectionAdded(smNode, conn);
            }
        }

        m_panning = Vector2::Zero;
        m_zoomScale = 1.0f;

        ImportCurrentAnimatorStateMachineExtraInformation();
        UpdatePanningAndZoomOnTransforms();
        CenterScene();
    }
}

Vector2 AnimatorSMEditorScene::GetMousePositionInSceneSpace() const
{
    return GetWorldPositionInSceneSpace( Vector2(Input::GetMousePosition()) );
}

Vector2 AnimatorSMEditorScene::GetWorldPositionInSceneSpace(
                                const Vector2 &pos) const
{
    RectTransform *contRT = p_mainContainer->GetRectTransform();
    Vector2 posLocal =
        contRT->FromLocalPointNDCToLocalPoint(
            contRT->FromWorldToLocalPoint(
                Vector3(Vector2(pos), 0) ).xy() );
    posLocal *= 1.0f / m_zoomScale;
    return posLocal;
}

void AnimatorSMEditorScene::CenterScene()
{
    m_panning = Vector2::Zero;
    SetZoomScale(1.0f);
    UpdatePanningAndZoomOnTransforms();

    AARect boundingRect;
    for (AESNode *aesNode : GetAESNodes())
    {
        boundingRect = AARect::Union(
                            boundingRect,
                            aesNode->GetRectTransform()->GetViewportAARect());
    }

    RectTransform *contRT = p_mainContainer->GetRectTransform();
    Vector2 boundingRectCenter = boundingRect.GetCenter();
    Vector2 contSize = contRT->GetViewportAARectWithoutTransform().GetCenter();
    m_panning = -boundingRectCenter + contSize;
    SetZoomScale(1.0f);
    UpdatePanningAndZoomOnTransforms();
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

void AnimatorSMEditorScene::SetZoomScale(float zoomScale)
{
    m_zoomScale = Math::Clamp(zoomScale, 0.1f, 2.0f);
}

void AnimatorSMEditorScene::UpdatePanningAndZoomOnTransforms()
{
    RectTransform *mainContRT = p_mainContainer->GetRectTransform();
    mainContRT->SetLocalPosition( Vector3(Vector2(m_panning), 0.0f) );
    mainContRT->SetLocalScale( Vector3(Vector2(m_zoomScale), 1.0f) );
    p_gridContainer->GetRectTransform()->SetLocalPosition(
                               Vector3(Vector2(m_zoomScale), 0) );
    p_gridContainer->GetRectTransform()->SetLocalPosition(
                               Vector3(Vector2(m_panning), 0) );

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

void AnimatorSMEditorScene::ImportCurrentAnimatorStateMachineExtraInformation()
{
    if (GetAnimatorSM())
    {
        Path metaPath = MetaFilesManager::GetMetaFilepath(
                    GetAnimatorSM()->GetResourceFilepath() );
        if (metaPath.IsFile())
        {
            MetaNode meta;
            meta.Import( metaPath );
            Array<Vector2> nodesPos = meta.GetArray<Vector2>("NodePositions");
            for (uint i = 0; i < nodesPos.Size(); ++i)
            {
                GetAESNodes()[i]->ImportPosition(nodesPos[i]);
            }
        }
    }
}

void AnimatorSMEditorScene::ExportCurrentAnimatorStateMachine()
{
    if (GetAnimatorSM() && GetAnimatorSM()->GetResourceFilepath().IsFile())
    {
        Path metaPath = MetaFilesManager::GetMetaFilepath(
                                GetAnimatorSM()->GetResourceFilepath());
        GetAnimatorSM()->ExportMetaToFile(metaPath);

        {
            MetaNode meta;
            meta.Import(metaPath);
            Array<Vector2> nodesPos;
            for (AESNode *node : GetAESNodes())
            {
                nodesPos.PushBack( node->GetExportPosition() );
            }
            meta.SetArray<Vector2>("NodePositions", nodesPos);
            File::Write(metaPath, meta.ToString());
        }
    }
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

    AESNode *aesNodeToRemove = p_nodes[removedNodeIdx];
    GameObject::Destroy(aesNodeToRemove);
    p_nodes.RemoveByIndex(removedNodeIdx);
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

