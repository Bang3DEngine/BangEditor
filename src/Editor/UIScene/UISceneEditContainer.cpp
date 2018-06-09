#include "BangEditor/UISceneEditContainer.h"

#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/UICanvas.h"
#include "Bang/Resources.h"
#include "Bang/Extensions.h"
#include "Bang/MeshRenderer.h"
#include "Bang/RectTransform.h"
#include "Bang/UIDragDroppable.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/Selection.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/ExplorerItem.h"
#include "BangEditor/UISceneImage.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

UISceneEditContainer::UISceneEditContainer()
{
    ScenePlayer::GetInstance()->
            EventEmitter<IEventsScenePlayer>::RegisterListener(this);
    SceneManager::GetActive()->
            EventEmitter<IEventsSceneManager>::RegisterListener(this);

    GameObject *cameraPreviewGo = GameObjectFactory::CreateUIGameObject();
    p_cameraPreviewImg = cameraPreviewGo->AddComponent<UIImageRenderer>();
    p_cameraPreviewImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    p_cameraPreviewImg->SetVisible(false);

    cameraPreviewGo->AddComponent<UILayoutIgnorer>();
    cameraPreviewGo->SetParent(this);
}

UISceneEditContainer::~UISceneEditContainer()
{
}

void UISceneEditContainer::Update()
{
    GameObject::Update();
    SetScene( EditorSceneManager::GetOpenScene() );

    if ( NeedsToRenderSelectionFramebuffer() )
    {
        Scene *openScene = EditorSceneManager::GetOpenScene();
        SelectionFramebuffer *sfb = GetSelectionFramebuffer();
        sfb->PrepareNewFrameForRender(openScene);
    }
}

void UISceneEditContainer::Render(RenderPass rp, bool renderChildren)
{
    if (m_needToRenderPreviewImg)
    {
        RenderCameraPreviewIfSelected();
        m_needToRenderPreviewImg = false;
    }

    GEngine *ge = GEngine::GetInstance();
    ge->PushActiveRenderingCamera();

    if ( NeedsToRenderSelectionFramebuffer() )
    {
        EditorCamera *edCam = EditorCamera::GetInstance();
        SelectionFramebuffer *sfb = GetSelectionFramebuffer();
        Scene *openScene = EditorSceneManager::GetOpenScene();
        GEngine *ge = GEngine::GetInstance();
        if (sfb && ge && edCam && openScene)
        {
            GL::Push(GL::Pushable::VIEWPORT);

            GL::SetViewport( AARecti( GetRectTransform()->GetViewportAARect() ) );
            edCam->GetCamera()->Bind();
            edCam->BindSelectionFramebuffer();
            sfb->PrepareNewFrameForRender(openScene);
            sfb->RenderForSelectionBuffer(openScene);

            GL::Pop(GL::Pushable::VIEWPORT);
        }
    }

    ge->PopActiveRenderingCamera();

    GameObject::Render(rp, renderChildren);
}

void UISceneEditContainer::HandleSelection()
{
    if (Input::GetMouseButtonDown(MouseButton::LEFT))
    {
        UICanvas *canvas = UICanvas::GetActive(this);
        bool isOverScene = canvas->IsMouseOver(GetSceneImage(), true);
        if (isOverScene)
        {
            GameObject *selectedGameObject = Selection::GetOveredGameObject();
            if (selectedGameObject)
            {
                Editor::SelectGameObject(selectedGameObject, true);
            }
            else { Editor::SelectGameObject(nullptr, true); }
        }
    }
}

void UISceneEditContainer::RenderCameraPreviewIfSelected()
{
    GameObject *selectedGO = Editor::GetSelectedGameObject();
    Camera *selectedCamera = selectedGO ? selectedGO->GetComponent<Camera>() :
                                          nullptr;

    // Camera preview handling
    if (selectedCamera)
    {
        GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

        GBuffer *gbuffer = selectedCamera->GetGBuffer();

        // Set preview size
        RectTransform *rt = GetRectTransform();
        AARecti sceneContainerRect( rt->GetViewportRect() );
        Vector2i sceneContainerSize = sceneContainerRect.GetSize();

        Vector2i previewRectSize = sceneContainerSize / 4;
        AARecti previewRectPx(sceneContainerRect.GetMax(),
                              sceneContainerRect.GetMax() - Vector2i(previewRectSize));

        previewRectPx.SetMin(sceneContainerRect.GetMin());
        previewRectPx.SetMax(previewRectPx.GetMin() + Vector2i(previewRectSize));

        // Add a bit of margin
        const Vector2i marginsBotLeft = Vector2i(5);
        p_cameraPreviewImg->GetGameObject()->GetRectTransform()->SetAnchors(
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMin() + marginsBotLeft),
           rt->FromViewportPointToLocalPointNDC(previewRectPx.GetMax()));

        gbuffer->Bind();
        gbuffer->Resize(previewRectPx.GetWidth(), previewRectPx.GetHeight());

        // Render in the size of sceneEditContainer, since we have a miniature,
        // but the canvas must be the same as in the scenePlayContainer size!!!
        Scene *openScene = EditorSceneManager::GetOpenScene();
        GEngine::GetInstance()->Render(openScene, selectedCamera);

        Texture2D *camColorTexture = gbuffer->GetLastDrawnColorTexture();
        camColorTexture->SetWrapMode( GL::WrapMode::REPEAT );
        p_cameraPreviewImg->SetImageTexture( gbuffer->GetLastDrawnColorTexture() );

        GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    }
    p_cameraPreviewImg->SetVisible( selectedCamera != nullptr );
}

Camera* UISceneEditContainer::GetSceneCamera(Scene *scene)
{
    Camera *editorCamera = EditorCamera::GetInstance()->GetCamera();
    if (editorCamera) { return editorCamera; }
    return scene->GetCamera();
}

bool UISceneEditContainer::NeedsToRenderScene(Scene *scene)
{
    return IsVisible();
}

void UISceneEditContainer::OnRenderNeededSceneFinished()
{
    m_needToRenderPreviewImg = true;
}

SelectionFramebuffer *UISceneEditContainer::GetSelectionFramebuffer() const
{
    EditorCamera *edCam = EditorCamera::GetInstance();
    return (edCam ? edCam->GetSelectionFramebuffer() : nullptr);
}

bool UISceneEditContainer::NeedsToRenderSelectionFramebuffer() const
{
    return (IsVisible() &&
            GetSceneImage()->GetRectTransform()->IsMouseOver(false));
}

GameObject* UISceneEditContainer::GetCurrentOveredGameObject() const
{
    GameObject *overedGameObject =
                  Selection::GetOveredGameObject( Input::GetMousePosition() );
    ASSERT(!overedGameObject->HasComponent<NotSelectableInEditor>());
    return overedGameObject;
}

void UISceneEditContainer::ApplyDraggedMaterialToOveredGameObject()
{
    GameObject *overedGameObject = GetCurrentOveredGameObject();
    if (overedGameObject)
    {
        List<MeshRenderer*> mrs = overedGameObject->GetComponents<MeshRenderer>();
        for (MeshRenderer *mr : mrs)
        {
            RH<Material> prevMat = RH<Material>(mr->GetActiveMaterial());
            mr->SetMaterial( m_currentMaterialBeingDragged.Get() );
            m_meshRenderersToPreviousMaterials.Add(mr, prevMat);
        }
    }
}

void UISceneEditContainer::RestoreDraggedMaterialToPreviousGameObjectOvered()
{
    List<MeshRenderer*> restoredMeshRenderers;
    for (const auto &pair : m_meshRenderersToPreviousMaterials)
    {
        MeshRenderer *mr = pair.first;
        RH<Material> previousMat = pair.second;
        mr->SetMaterial( previousMat.Get() );
        restoredMeshRenderers.PushBack(mr);
    }

    for (MeshRenderer *mr : restoredMeshRenderers)
    {
        m_meshRenderersToPreviousMaterials.Remove(mr);
    }
}

void UISceneEditContainer::OnDragStarted(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragStarted(dd_);

    UIDragDroppable *dragDroppable = DCAST<UIDragDroppable*>(dd_);
    ExplorerItem *expItem = DCAST<ExplorerItem*>(dragDroppable->GetGameObject());
    if (expItem)
    {
        Path draggedPath = expItem->GetPath();
        if (draggedPath.HasExtension( Extensions::GetMaterialExtension() ))
        {
            m_currentMaterialBeingDragged = Resources::Load<Material>(draggedPath);
            p_lastOveredGameObject = GetCurrentOveredGameObject();

            ApplyDraggedMaterialToOveredGameObject();
        }
    }

    if (m_currentMaterialBeingDragged)
    {
        Camera *edCam = EditorCamera::GetInstance()->GetCamera();
        edCam->RemoveRenderPass(RenderPass::OVERLAY);
    }
}

void UISceneEditContainer::OnDragUpdate(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDragUpdate(dd_);

    if (m_currentMaterialBeingDragged)
    {
        GameObject *currentOveredGameObject = GetCurrentOveredGameObject();
        if (currentOveredGameObject != p_lastOveredGameObject)
        {
            RestoreDraggedMaterialToPreviousGameObjectOvered();

            p_lastOveredGameObject = currentOveredGameObject;
            ApplyDraggedMaterialToOveredGameObject();
        }
    }
}

void UISceneEditContainer::OnDrop(EventEmitter<IEventsDragDrop> *dd_)
{
    IEventsDragDrop::OnDrop(dd_);

    m_currentMaterialBeingDragged.Set(nullptr);
    m_meshRenderersToPreviousMaterials.Clear();

    Camera *edCam = EditorCamera::GetInstance()->GetCamera();
    edCam->AddRenderPass(RenderPass::OVERLAY);
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
        m_meshRenderersToPreviousMaterials.Remove(mr);
    }
}

void UISceneEditContainer::OnPlayStateChanged(PlayState, PlayState)
{
}

void UISceneEditContainer::OnSceneLoaded(Scene *scene, const Path&)
{
}
