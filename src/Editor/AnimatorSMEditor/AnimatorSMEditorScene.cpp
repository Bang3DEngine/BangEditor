#include "BangEditor/AnimatorSMEditorScene.h"

#include "Bang/AnimatorStateMachine.h"
#include "Bang/AnimatorStateMachineLayer.h"
#include "Bang/AnimatorStateMachineNode.h"
#include "Bang/Assert.h"
#include "Bang/Color.h"
#include "Bang/Debug.h"
#include "Bang/EventEmitter.h"
#include "Bang/File.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsAnimatorStateMachine.h"
#include "Bang/Input.h"
#include "Bang/Material.h"
#include "Bang/Math.h"
#include "Bang/MetaFilesManager.h"
#include "Bang/MetaNode.h"
#include "Bang/MetaNode.tcc"
#include "Bang/MouseButton.h"
#include "Bang/Path.h"
#include "Bang/RectTransform.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIRectMask.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "BangEditor/AESConnectionLine.h"
#include "BangEditor/AESNode.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/MenuItem.h"
#include "BangEditor/UIContextMenu.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

namespace Bang
{
class AnimatorStateMachineTransition;
class IEventsAnimatorStateMachineNode;
}

using namespace Bang;
using namespace BangEditor;

AnimatorSMEditorScene::AnimatorSMEditorScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    AddComponent<UIRectMask>();

    UIImageRenderer *nodesSceneBG = AddComponent<UIImageRenderer>();
    nodesSceneBG->SetTint(Color::DarkGray);

    GameObject *borderGo = GameObjectFactory::CreateUIGameObject();
    p_border = GameObjectFactory::AddInnerBorder(borderGo);
    borderGo->GetRectTransform()->SetLocalPosition(Vector3(0, 0, -0.04f));
    borderGo->SetParent(this);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->EventEmitter<IEventsFocus>::RegisterListener(this);

    p_zoomContainer = GameObjectFactory::CreateUIGameObject();
    p_zoomContainer->SetParent(this);

    p_mainContainer = GameObjectFactory::CreateUIGameObject();
    p_mainContainer->GetRectTransform()->SetPivotPosition(Vector2::Zero);
    p_mainContainer->SetParent(p_zoomContainer);

    p_gridContainer = GameObjectFactory::CreateUIGameObject();
    p_gridContainer->GetRectTransform()->SetAnchors(Vector2::Zero);
    p_gridContainer->GetRectTransform()->SetPivotPosition(Vector2::Zero);
    p_gridContainer->GetRectTransform()->SetSizeFromPivot(Vector2i(100000));
    p_gridContainer->SetParent(this);

    p_gridImg = p_gridContainer->AddComponent<UIImageRenderer>();
    p_gridImg->SetImageTexture(EditorTextureFactory::GetGrid2x2());
    p_gridImg->GetMaterial()->SetAlbedoUvMultiply(Vector2(10000.0f));

    p_contextMenu = AddComponent<UIContextMenu>();
    p_contextMenu->SetCreateContextMenuCallback([this](MenuItem *menuRootItem) {
        if (GetAnimatorSM() && !IsMouseOverSomeConnectionLine())
        {
            menuRootItem->SetFontSize(12);

            MenuItem *createNode = menuRootItem->AddItem("Create new node");
            createNode->SetSelectedCallback([this](MenuItem *) {
                MetaNode previousMeta = GetAnimatorSM()->GetMeta();

                GetAnimatorSMLayer()->CreateAndAddNode();

                UndoRedoManager::PushAction(new UndoRedoSerializableChange(
                    GetAnimatorSM(), previousMeta, GetAnimatorSM()->GetMeta()));

                AESNode *aesNode = p_nodes[p_nodes.Size() - 1];
                RectTransform *aesNodeRT = aesNode->GetRectTransform();
                float localPosZ = aesNodeRT->GetLocalPosition().z;
                Vector3 localPos =
                    Vector3(GetMousePositionInSceneSpace(), localPosZ);
                aesNodeRT->SetLocalPosition(localPos);
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

    // Center scene gracefully...xd
    if (IsVisibleRecursively())
    {
        if (++m_framesAfterNewAnimatorSMSetAndVisible == 5)
        {
            CenterScene();
        }
    }
    p_mainContainer->SetVisible(m_framesAfterNewAnimatorSMSetAndVisible >= 5);

    if (UICanvas::GetActive(this)->HasFocus(this, true))
    {
        if (Input::GetMouseButton(MouseButton::MIDDLE))
        {
            Vector2 mouseDelta(Input::GetMouseDelta());
            m_panning += mouseDelta;
        }
    }

    if (Time::GetPassedTimeSince(m_lastTimeAnimatorSMWasExported) >=
        Time::Seconds(1.0f))
    {
        ExportCurrentAnimatorStateMachineIfAny();
    }

    UpdatePanningAndZoomOnTransforms();
}

void AnimatorSMEditorScene::CreateAndAddNode(AnimatorStateMachineNode *smNode,
                                             uint addIdx)
{
    AESNode *aesNode = new AESNode();
    aesNode->p_aesScene = this;
    aesNode->SetSMNode(smNode);
    aesNode->SetParent(p_mainContainer);

    smNode->EventEmitter<IEventsAnimatorStateMachineNode>::RegisterListener(
        aesNode);

    ASSERT(addIdx <= p_nodes.Size());
    p_nodes.Insert(aesNode, addIdx);

    UpdatePanningAndZoomOnTransforms();
}

void AnimatorSMEditorScene::SetAnimatorSMLayer(
    AnimatorStateMachineLayer *animatorSMLayer)
{
    if (animatorSMLayer != GetAnimatorSMLayer())
    {
        ExportCurrentAnimatorStateMachineIfAny();

        Clear();
        p_animatorSMLayer = animatorSMLayer;

        if (GetAnimatorSMLayer())
        {
            GetAnimatorSMLayer()
                ->EventEmitter<
                    IEventsAnimatorStateMachineLayer>::RegisterListener(this);

            for (uint i = 0; i < GetAnimatorSMLayer()->GetNodes().Size(); ++i)
            {
                AnimatorStateMachineNode *smNode = animatorSMLayer->GetNode(i);
                ASSERT(smNode);

                OnNodeCreated(i, smNode);
            }

            for (uint i = 0; i < GetAnimatorSMLayer()->GetNodes().Size(); ++i)
            {
                AESNode *aesNode = GetAESNodes()[i];
                AnimatorStateMachineNode *smNode =
                    GetAnimatorSMLayer()->GetNode(i);
                for (AnimatorStateMachineTransition *conn :
                     GetAnimatorSMLayer()->GetNode(i)->GetTransitions())
                {
                    aesNode->OnTransitionAdded(smNode, conn);
                }
            }

            ImportCurrentAnimatorStateMachineExtraInformation();
        }
        m_framesAfterNewAnimatorSMSetAndVisible = 0;
        p_mainContainer->SetVisible(false);  // To handle scene centering nice
    }
}

Vector2 AnimatorSMEditorScene::GetMousePositionInSceneSpace() const
{
    return GetWorldPositionInSceneSpace(Vector2(Input::GetMousePosition()));
}

Vector2 AnimatorSMEditorScene::GetWorldPositionInSceneSpace(
    const Vector2 &pos) const
{
    RectTransform *contRT = p_mainContainer->GetRectTransform();
    Vector2 posLocal = contRT->FromLocalPointNDCToLocalPoint(
        contRT->FromWorldToLocalPoint(Vector3(Vector2(pos), 0)).xy());
    posLocal *= 1.0f / GetZoomScale();
    return posLocal;
}

void AnimatorSMEditorScene::CenterScene()
{
    m_panning = Vector2::Zero;
    UpdatePanningAndZoomOnTransforms();
    SetZoomScale(1.0f, false);
    UpdatePanningAndZoomOnTransforms();

    AARect boundingRect;
    for (AESNode *aesNode : GetAESNodes())
    {
        boundingRect = AARect::Union(
            boundingRect, aesNode->GetRectTransform()->GetViewportAARect());
    }

    RectTransform *contRT = p_mainContainer->GetRectTransform();
    Vector2 boundingRectCenter = boundingRect.GetCenter();
    Vector2 boundingRectSize = boundingRect.GetSize();
    Vector2 contSize = contRT->GetViewportAARectWithoutTransform().GetCenter();

    m_panning = -boundingRectCenter + contSize;
    UpdatePanningAndZoomOnTransforms();

    Vector2 zoomVec = (contSize / boundingRectSize);
    float zoom = Math::Min(zoomVec.x, zoomVec.y);
    zoom = Math::Min(zoom, 1.0f);
    SetZoomScale(zoom, false);

    UpdatePanningAndZoomOnTransforms();
}

void AnimatorSMEditorScene::Clear()
{
    if (GetAnimatorSMLayer())
    {
        GetAnimatorSMLayer()
            ->EventEmitter<
                IEventsAnimatorStateMachineLayer>::UnRegisterListener(this);
    }

    p_animatorSMLayer = nullptr;

    for (AESNode *node : p_nodes)
    {
        GameObject::Destroy(node);
    }
    p_nodes.Clear();
}

float AnimatorSMEditorScene::GetZoomScale() const
{
    return m_zoomScale;
}

const Array<AESNode *> &AnimatorSMEditorScene::GetAESNodes() const
{
    return p_nodes;
}

AnimatorStateMachine *AnimatorSMEditorScene::GetAnimatorSM() const
{
    return GetAnimatorSMLayer() ? GetAnimatorSMLayer()->GetStateMachine()
                                : nullptr;
}

AnimatorStateMachineLayer *AnimatorSMEditorScene::GetAnimatorSMLayer() const
{
    return p_animatorSMLayer;
}

void AnimatorSMEditorScene::SetZoomScale(float zoomScale, bool centerOnMouse)
{
    if (zoomScale != GetZoomScale())
    {
        m_zoomScale = Math::Clamp(zoomScale, 0.1f, 2.0f);

        RectTransform *zoomContRT = p_zoomContainer->GetRectTransform();
        if (zoomScale != 1.0f)
        {
            Vector2 mousePosWorld(Input::GetMousePosition());

            Vector2 prevMousePosLocal =
                zoomContRT->FromWorldToLocalPoint(mousePosWorld.xy0()).xy();

            zoomContRT->SetLocalScale(GetZoomScale());

            if (centerOnMouse)
            {
                Vector2 newMousePosLocal =
                    zoomContRT->FromWorldToLocalPoint(mousePosWorld.xy0()).xy();

                Vector2 correctionDisplacementLocal =
                    (prevMousePosLocal - newMousePosLocal);
                Vector2 correctionDisplacementWorld =
                    zoomContRT
                        ->FromLocalToWorldVector(
                            correctionDisplacementLocal.xy0())
                        .xy();

                zoomContRT->SetLocalPosition(zoomContRT->GetLocalPosition() -
                                             correctionDisplacementWorld.xy0());
            }
            else
            {
                zoomContRT->SetLocalPosition(Vector3::Zero);
            }
        }
        else
        {
            zoomContRT->SetLocalScale(1.0f);
            zoomContRT->SetLocalPosition(Vector3::Zero);
        }
    }
}

void AnimatorSMEditorScene::UpdatePanningAndZoomOnTransforms()
{
    RectTransform *mainContRT = p_mainContainer->GetRectTransform();
    mainContRT->SetLocalPosition(Vector3(Vector2(m_panning), 0.0f));

    // p_gridContainer->GetRectTransform()->SetLocalScale(
    //     Vector3(Vector2(GetZoomScale()), 0));
    p_gridContainer->GetRectTransform()->SetLocalPosition(
        Vector3(Vector2(m_panning), 0));

    for (AESNode *node : p_nodes)
    {
        node->OnZoomScaleChanged(GetZoomScale());
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

Path AnimatorSMEditorScene::GetAnimatorSMExtraInfoPath() const
{
    if (GetAnimatorSM())
    {
        Path resourcePath = GetAnimatorSM()->GetResourceFilepath();
        if (resourcePath.IsFile())
        {
            Path metaResourcePath =
                MetaFilesManager::GetMetaFilepath(resourcePath);
            return Path(
                metaResourcePath.GetAbsolute().Replace(".meta", ".metaed"));
        }
    }
    return Path::Empty;
}

void AnimatorSMEditorScene::ImportCurrentAnimatorStateMachineExtraInformation()
{
    if (AnimatorStateMachine *sm = GetAnimatorSM())
    {
        Path extraInfoPath = GetAnimatorSMExtraInfoPath();
        if (extraInfoPath.IsFile())
        {
            MetaNode extraInfoMeta;
            extraInfoMeta.Import(File::GetContents(extraInfoPath));

            const Array<AnimatorStateMachineLayer *> &layers = sm->GetLayers();
            for (uint i = 0; i < layers.Size(); ++i)
            {
                if (MetaNode *layerMeta = extraInfoMeta.GetChild("Layers", i))
                {
                    AnimatorStateMachineLayer *layer = layers[i];
                    if (layer == GetAnimatorSMLayer())
                    {
                        const auto &aesNodes = GetAESNodes();
                        for (uint j = 0; j < aesNodes.Size(); ++j)
                        {
                            if (MetaNode *nodeExtraMeta =
                                    layerMeta->GetChild("Nodes", j))
                            {
                                AESNode *aesNode = aesNodes[j];
                                Vector2 aesNodePos =
                                    nodeExtraMeta->Get<Vector2>("Position");
                                aesNode->ImportPosition(aesNodePos);
                            }
                        }
                    }
                }
            }
        }
    }
}

void AnimatorSMEditorScene::ExportCurrentAnimatorStateMachineIfAny()
{
    if (AnimatorStateMachine *sm = GetAnimatorSM())
    {
        Path extraInfoPath = GetAnimatorSMExtraInfoPath();
        MetaNode extraInfoMeta;
        extraInfoMeta.Import(File::GetContents(extraInfoPath));

        const Array<AnimatorStateMachineLayer *> &layers = sm->GetLayers();
        for (uint i = 0; i < layers.Size(); ++i)
        {
            AnimatorStateMachineLayer *layer = layers[i];
            MetaNode *layerMeta = extraInfoMeta.GetChild("Layers", i);
            if (!layerMeta)
            {
                extraInfoMeta.AddChild(MetaNode(), "Layers");
                layerMeta = extraInfoMeta.GetChild("Layers", i);
            }
            layerMeta->SetName(layer->GetLayerName());

            if (layer == GetAnimatorSMLayer())
            {
                ASSERT(layerMeta);
                layerMeta->RemoveChildren("Nodes");

                const auto &aesNodes = GetAESNodes();
                for (uint j = 0; j < aesNodes.Size(); ++j)
                {
                    MetaNode nodeExtraMeta;
                    AESNode *aesNode = aesNodes[j];
                    nodeExtraMeta.SetName(aesNode->GetNodeName());
                    nodeExtraMeta.Set(
                        "Position",
                        aesNode->GetRectTransform()->GetLocalPosition().xy());
                    layerMeta->AddChild(nodeExtraMeta, "Nodes");
                }
            }
        }

        File::Write(extraInfoPath, extraInfoMeta.ToString());

        m_lastTimeAnimatorSMWasExported = Time::GetNow();
    }
}

void AnimatorSMEditorScene::OnNodeCreated(uint newNodeIdx,
                                          AnimatorStateMachineNode *newNode)
{
    CreateAndAddNode(newNode, newNodeIdx);
}

void AnimatorSMEditorScene::OnNodeRemoved(uint removedNodeIdx,
                                          AnimatorStateMachineNode *removedNode)
{
    BANG_UNUSED(removedNode);
    ASSERT(removedNodeIdx < p_nodes.Size());

    AESNode *aesNodeToRemove = p_nodes[removedNodeIdx];
    GameObject::Destroy(aesNodeToRemove);
    p_nodes.RemoveByIndex(removedNodeIdx);
}

UIEventResult AnimatorSMEditorScene::OnUIEvent(UIFocusable *,
                                               const UIEvent &event)
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

        case UIEvent::Type::WHEEL:
        {
            Vector2 mouseWheel = (Input::GetMouseWheel() * Vector2(0.05f));
            if (mouseWheel != Vector2::Zero)
            {
                SetZoomScale(GetZoomScale() + mouseWheel.y, true);
                return UIEventResult::INTERCEPT;
            }
        }
        break;

        default: break;
    }
    return UIEventResult::IGNORE;
}
