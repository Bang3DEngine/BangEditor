#include "BangEditor/UISceneEditContainer.h"

#include <utility>

#include "Bang/AARect.h"
#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/Camera.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/Extensions.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsGameObjectVisibilityChanged.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/List.h"
#include "Bang/List.tcc"
#include "Bang/Map.tcc"
#include "Bang/Material.h"
#include "Bang/MeshRenderer.h"
#include "Bang/MouseButton.h"
#include "Bang/Path.h"
#include "Bang/RectTransform.h"
#include "Bang/Resources.h"
#include "Bang/Resources.tcc"
#include "Bang/Scene.h"
#include "Bang/SceneManager.h"
#include "Bang/Texture2D.h"
#include "Bang/UICanvas.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector2.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorScene.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/GizmosManager.h"
#include "BangEditor/Hierarchy.h"
#include "BangEditor/HierarchyItem.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/Selection.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

namespace Bang {
class IEventsDestroy;
}  // namespace Bang

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneEditContainer::UISceneEditContainer()
{
    GameObject *cameraPreviewGo = GameObjectFactory::CreateUIGameObject();
    p_cameraPreviewImg = cameraPreviewGo->AddComponent<UIImageRenderer>();
    p_cameraPreviewImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    cameraPreviewGo->SetVisible(false);

    GameObjectFactory::AddOuterBorder(cameraPreviewGo);

    m_cameraPreviewGBuffer = new GBuffer(1,1);

    cameraPreviewGo->GetRectTransform()->SetAnchors( Vector2(-1),
                                                     Vector2(-1) );
    cameraPreviewGo->GetRectTransform()->SetMarginLeftBot( Vector2i(5) );
    cameraPreviewGo->AddComponent<UILayoutIgnorer>();
    cameraPreviewGo->SetParent( GetSceneImage() );

    ScenePlayer::GetInstance()->
            EventEmitter<IEventsScenePlayer>::RegisterListener(this);
    SceneManager::GetActive()->
            EventEmitter<IEventsSceneManager>::RegisterListener(this);
    EventEmitter<IEventsGameObjectVisibilityChanged>::RegisterListener(this);
}

UISceneEditContainer::~UISceneEditContainer()
{
    delete m_cameraPreviewGBuffer;
}

void UISceneEditContainer::Update()
{
    GameObject::Update();
    SetScene( EditorSceneManager::GetOpenScene() );
}

void UISceneEditContainer::Render(RenderPass rp, bool renderChildren)
{
    if (m_needToRenderPreviewImg && (rp == RenderPass::CANVAS) )
    {
        RenderCameraPreviewIfSelected();
        m_needToRenderPreviewImg = false;
    }

    if ( rp == RenderPass::CANVAS && NeedsToRenderSelectionFramebuffer() )
    {
        EditorCamera *edCamGo = EditorCamera::GetInstance();
        Camera *edCam = edCamGo->GetCamera();
        SelectionFramebuffer *sfb = GetSelectionFramebuffer();
        Scene *openScene = EditorSceneManager::GetOpenScene();
        GEngine *ge = GEngine::GetInstance();
        if (sfb && ge && edCamGo && openScene)
        {
            GEngine *ge = GEngine::GetInstance();
            ge->PushActiveRenderingCamera();

            GizmosManager *sgm = GizmosManager::GetInstance();
            sgm->OnBeginRender(openScene);
            ge->PushActiveRenderingCamera();
            ge->SetActiveRenderingCamera(edCam);

            RectTransform *sceneImgRT = GetSceneImage()->GetRectTransform();
            AARecti imgRect( sceneImgRT->GetViewportAARect() );
            Vector2i renderSize = imgRect.GetSize();
            edCam->SetRenderSize(renderSize);
            edCamGo->GetSelectionFramebuffer()->Resize(renderSize);

            edCam->Bind();

            sfb->PrepareNewFrameForRender(openScene);
            sfb->RenderForSelectionBuffer(openScene);

            edCam->UnBind();

            ge->PopActiveRenderingCamera();
            sgm->OnEndRender(openScene);

            ge->PopActiveRenderingCamera();
        }
    }

    UISceneContainer::Render(rp, renderChildren);
}

bool UISceneEditContainer::HasFocus()
{
    UISceneEditContainer *uisec = UISceneEditContainer::GetActive();
    return uisec->GetFocusable()->HasFocus();
}

bool UISceneEditContainer::IsMouseOver()
{
    UISceneEditContainer *uisec = UISceneEditContainer::GetActive();
    UICanvas *canvas = UICanvas::GetActive(uisec);
    return canvas ? canvas->IsMouseOver(uisec->GetFocusable()) : false;
}

Vector2i UISceneEditContainer::GetMousePositionInOpenScene()
{
    Vector2i mousePosInOpenScene = Vector2i::Zero;
    if (UISceneEditContainer *sec = UISceneEditContainer::GetActive())
    {
        AARecti vp = GL::GetViewportRect();
        AARecti sceneVPRect( sec->GetSceneImage()->GetRectTransform()->
                             GetViewportAARect() );
        AARecti windowSceneRect = sceneVPRect + vp.GetMin();
        Vector2i vpMousePos( GL::FromWindowPointToViewportPoint(
                                Vector2(Input::GetMousePositionWindow()),
                                windowSceneRect) );
        mousePosInOpenScene = vpMousePos;
    }
    return mousePosInOpenScene;
}

void UISceneEditContainer::RenderCameraPreviewIfSelected()
{
    GameObject *selectedGO = Editor::GetSelectedGameObject();
    Camera *selectedCamera = selectedGO ? selectedGO->GetComponent<Camera>() :
                                          nullptr;

    // Camera preview handling
    if (selectedCamera)
    {
        // Set preview size
        Vector2i previewRectSize( GetSceneImage()->GetRectTransform()->
                                  GetViewportAARect().GetSize() / 4.0f );
        if (previewRectSize.x > 0 && previewRectSize.y > 0)
        {
            GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

            GBuffer *gbuffer = m_cameraPreviewGBuffer;

            p_cameraPreviewImg->GetGameObject()->GetRectTransform()->
                                SetMarginRightTop(-previewRectSize);

            m_cameraPreviewGBuffer->Bind();
            m_cameraPreviewGBuffer->Resize(previewRectSize.x,
                                           previewRectSize.y);

            Scene *openScene = EditorSceneManager::GetOpenScene();

            selectedCamera->SetReplacementGBuffer(gbuffer);
            GEngine::GetInstance()->Render(openScene, selectedCamera);
            selectedCamera->SetReplacementGBuffer(nullptr);

            Texture2D *camColorTexture = gbuffer->GetLastDrawnColorTexture();
            camColorTexture->SetWrapMode( GL::WrapMode::REPEAT );
            p_cameraPreviewImg->SetImageTexture(camColorTexture);

            GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
        }
    }
    p_cameraPreviewImg->GetGameObject()->SetVisible( selectedCamera != nullptr );
}

Camera* UISceneEditContainer::GetSceneCamera(Scene *scene)
{
    Camera *editorCamera = EditorCamera::GetInstance()->GetCamera();
    if (editorCamera)
    {
        return editorCamera;
    }
    return scene->GetCamera();
}

bool UISceneEditContainer::NeedsToRenderContainedScene(Scene *scene)
{
    BANG_UNUSED(scene);
    return IsVisibleRecursively();
}

void UISceneEditContainer::OnRenderContainedSceneBegin()
{
    EditorSceneManager::GetActive()->GetEditorScene()->
          GetEditSceneGameObjects()->OnBeginRender(
                EditorSceneManager::GetActive()->GetOpenScene() );
}

void UISceneEditContainer::OnRenderContainedSceneFinished()
{
    EditorSceneManager::GetActive()->GetEditorScene()->
          GetEditSceneGameObjects()->OnEndRender(
                EditorSceneManager::GetActive()->GetOpenScene() );

    m_needToRenderPreviewImg = true;
}

SelectionFramebuffer *UISceneEditContainer::GetSelectionFramebuffer() const
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return (edCam ? edCam->GetSelectionFramebuffer() : nullptr);
}

bool UISceneEditContainer::NeedsToRenderSelectionFramebuffer() const
{
    return (IsVisibleRecursively() &&
            GetSceneImage()->GetRectTransform()->IsMouseOver(false));
}

GameObject* UISceneEditContainer::GetCurrentOveredGameObject() const
{
    AARecti imgRect( GetSceneImage()->GetRectTransform()->GetViewportAARect() );
    Vector2i mousePosLocal = Input::GetMousePosition() - imgRect.GetMin();
    GameObject *overedGameObject = Selection::GetOveredGameObject(mousePosLocal);
    ASSERT(!overedGameObject ||
           !overedGameObject->HasComponent<NotSelectableInEditor>());
    return overedGameObject;
}

void UISceneEditContainer::ApplyDraggedMaterialToOveredGameObject()
{
    if (GameObject *overedGameObject = p_lastOveredGameObject)
    {
        Array<MeshRenderer*> mrs = overedGameObject->GetComponents<MeshRenderer>();
        for (MeshRenderer *mr : mrs)
        {
            RH<Material> prevMat = RH<Material>(mr->GetActiveMaterial());
            mr->SetMaterial( m_currentMaterialBeingDragged.Get() );
            m_matDragMeshRenderersToPrevMaterials.Add(mr, prevMat);
        }
    }
}

void UISceneEditContainer::RestoreDraggedMaterialToPreviousGameObjectOvered()
{
    List<MeshRenderer*> restoredMeshRenderers;
    for (const auto &pair : m_matDragMeshRenderersToPrevMaterials)
    {
        MeshRenderer *mr = pair.first;
        RH<Material> previousMat = pair.second;
        mr->SetMaterial( previousMat.Get() );
        restoredMeshRenderers.PushBack(mr);
    }

    for (MeshRenderer *mr : restoredMeshRenderers)
    {
        m_matDragMeshRenderersToPrevMaterials.Remove(mr);
    }
}

void UISceneEditContainer::OnVisibilityChanged(GameObject*)
{
    EditorCamera *edCamGo = EditorCamera::GetInstance();
    if (IsVisible())
    {
        edCamGo->RequestUnBlockBy(this);
    }
    else
    {
        edCamGo->RequestBlockBy(this);
    }
}

UIEventResult UISceneEditContainer::OnUIEvent(UIFocusable *focusable,
                                              const UIEvent &event)
{
    if (UISceneContainer::OnUIEvent(focusable, event) == UIEventResult::INTERCEPT)
    {
        return UIEventResult::INTERCEPT;
    }

    switch (event.type)
    {
        case UIEvent::Type::MOUSE_CLICK_DOWN:
        {
            if (event.mouse.button == MouseButton::LEFT)
            {
                if (GameObject *selectedGo = Selection::GetOveredGameObject())
                {
                    Editor::SelectGameObject(selectedGo, true);
                }
                else
                {
                    Editor::SelectGameObject(nullptr, true);
                }
                return UIEventResult::INTERCEPT;
            }
        }
        break;

        case UIEvent::Type::KEY_DOWN:
            switch (event.key.key)
            {
                case Key::C:
                case Key::X:
                case Key::V:
                case Key::D:
                case Key::F:
                case Key::DELETE:
                    if (Hierarchy *hierarchy = Hierarchy::GetInstance())
                    {
                        if (GameObject *selGo = Editor::GetSelectedGameObject() )
                        {
                            if (HierarchyItem *hItem =
                                    hierarchy->GetItemFromGameObject(selGo))
                            {
                                return hItem->OnUIEvent(focusable, event);
                            }
                        }
                        return hierarchy->OnUIEvent(focusable, event);
                    }
                break;

                default:
                break;
            }
        break;

        default:
        break;
    }
    return UIEventResult::IGNORE;
}

void UISceneEditContainer::OnDragStarted(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragStarted(dd_);

    UIDragDroppable *dragDroppable = DCAST<UIDragDroppable*>(dd_);
    if (ExplorerItem *expItem = DCAST<ExplorerItem*>(dragDroppable->GetGameObject()))
    {
        Path draggedPath = expItem->GetPath();
        if (draggedPath.HasExtension( Extensions::GetMaterialExtension() ))
        {
            m_currentMaterialBeingDragged = Resources::Load<Material>(draggedPath);
            if (m_currentMaterialBeingDragged)
            {
                Camera *edCam = EditorCamera::GetInstance()->GetCamera();
                edCam->RemoveRenderPass(RenderPass::OVERLAY);
                GetSelectionFramebuffer()->SetDrawOverlay(false);
            }

        }
    }

}

void UISceneEditContainer::OnDragUpdate(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragUpdate(dd_);

    if (m_currentMaterialBeingDragged)
    {
        GameObject *currentOveredGo = GetCurrentOveredGameObject();
        if (currentOveredGo != p_lastOveredGameObject)
        {
            RestoreDraggedMaterialToPreviousGameObjectOvered();

            m_prevGameObjectMetaBeforeDraggingMaterial =
                    currentOveredGo ? currentOveredGo->GetMeta() : MetaNode();
            p_lastOveredGameObject = currentOveredGo;
            ApplyDraggedMaterialToOveredGameObject();
        }
    }
}

void UISceneEditContainer::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDrop(dd_);

    if (p_lastOveredGameObject)
    {
        #ifdef DEBUG
        GUID prevGUID = m_prevGameObjectMetaBeforeDraggingMaterial.Get<GUID>("GUID");
        GUID currGUID = p_lastOveredGameObject->GetGUID();
        ASSERT(prevGUID == currGUID);
        #endif
        UndoRedoManager::PushAction( new UndoRedoSerializableChange(
                                p_lastOveredGameObject,
                                m_prevGameObjectMetaBeforeDraggingMaterial,
                                p_lastOveredGameObject->GetMeta()));
    }
    p_lastOveredGameObject = nullptr;
    m_currentMaterialBeingDragged.Set(nullptr);
    m_matDragMeshRenderersToPrevMaterials.Clear();
    m_prevGameObjectMetaBeforeDraggingMaterial = MetaNode();

    Camera *edCam = EditorCamera::GetInstance()->GetCamera();
    edCam->AddRenderPass(RenderPass::OVERLAY);
    GetSelectionFramebuffer()->SetDrawOverlay(true);
}

void UISceneEditContainer::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    UISceneContainer::OnDestroyed(object);

    GameObject *go = DCAST<GameObject*>(object);
    if (go && p_lastOveredGameObject)
    {
        p_lastOveredGameObject = nullptr;
    }

    MeshRenderer *mr = DCAST<MeshRenderer*>(mr);
    if (mr)
    {
        m_matDragMeshRenderersToPrevMaterials.Remove(mr);
    }
}

void UISceneEditContainer::OnPlayStateChanged(PlayState, PlayState)
{
}

void UISceneEditContainer::OnSceneLoaded(Scene *scene, const Path&)
{
    BANG_UNUSED(scene);
}

UISceneEditContainer *UISceneEditContainer::GetActive()
{
    if (EditorSceneManager *esm = EditorSceneManager::GetActive())
    {
        if (EditorScene *edScene = esm->GetEditorScene())
        {
            if (UISceneEditContainer *sec = edScene->GetSceneEditContainer())
            {
                return sec;
            }
        }
    }
    return nullptr;
}
