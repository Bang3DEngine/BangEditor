#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include "Bang/Bang.h"
#include "Bang/Material.h"
#include "Bang/SceneManager.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IDestroyListener.h"
#include "Bang/IDragDropListener.h"

#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class MeshRenderer;
FORWARD class UIImageRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneEditContainer : public UISceneContainer,
                             public IScenePlayerListener,
                             public ISceneManagerListener,
                             public IDragDropListener
{
    GAMEOBJECT_EDITOR(UISceneEditContainer);

public:
	UISceneEditContainer();
	virtual ~UISceneEditContainer();

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    void HandleSelection();

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

    // Material drag related
    GameObject* GetCurrentOveredGameObject() const;
    void ApplyDraggedMaterialToOveredGameObject();
    void RestoreDraggedMaterialToPreviousGameObjectOvered();

    // IDestroyListener
    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

    // IDragDropListener
    virtual void OnDragStarted(UIDragDroppable *dragDroppable) override;
    virtual void OnDragUpdate(UIDragDroppable *dragDroppable) override;
    virtual void OnDrop(UIDragDroppable *dragDroppable) override;

    // IScenePlayerListener
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // ISceneManagerListener
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENEEDITCONTAINER_H

