#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/Material.h"
#include "Bang/UIFocusable.h"
#include "Bang/SceneManager.h"
#include "Bang/EventListener.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsDragDrop.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class SelectionFramebuffer;

class UISceneEditContainer :
        public UISceneContainer,
        public EventListener<IEventsGameObjectVisibilityChanged>,
        public EventListener<IEventsScenePlayer>,
        public EventListener<IEventsSceneManager>,
        public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR(UISceneEditContainer);

public:
	UISceneEditContainer();
	virtual ~UISceneEditContainer();

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    void HandleSelection();
    static Vector2i GetMousePositionInOpenScene();

private:
    bool m_needToRenderPreviewImg = false;
    UIImageRenderer *p_cameraPreviewImg = nullptr;

    RH<Material> m_currentMaterialBeingDragged;
    GameObject *p_lastOveredGameObject = nullptr;
    Map<MeshRenderer*, RH<Material>> m_meshRenderersToPreviousMaterials;

    void RenderCameraPreviewIfSelected();
    Camera* GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderScene(Scene *scene) override;
    void OnRenderNeededSceneFinished() override;

    SelectionFramebuffer *GetSelectionFramebuffer() const;
    bool NeedsToRenderSelectionFramebuffer() const;

    // Material drag related
    GameObject* GetCurrentOveredGameObject() const;
    void ApplyDraggedMaterialToOveredGameObject();
    void RestoreDraggedMaterialToPreviousGameObjectOvered();

    // IEventsGameObjectVisibilityChanged
    void OnVisibilityChanged(GameObject *go) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    // IEventsDragDrop
    virtual void OnDragStarted(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable,
                        bool inside) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEEDITCONTAINER_H

