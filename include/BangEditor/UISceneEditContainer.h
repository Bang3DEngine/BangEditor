#ifndef UISCENEEDITCONTAINER_H
#define UISCENEEDITCONTAINER_H

#include <functional>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/IEventsDragDrop.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsGameObjectVisibilityChanged.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Map.h"
#include "Bang/Material.h"
#include "Bang/MetaNode.h"
#include "Bang/RenderPass.h"
#include "Bang/SceneManager.h"
#include "Bang/String.h"
#include "Bang/UIFocusable.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneContainer.h"

namespace Bang
{
class Camera;
class GameObject;
class GBuffer;
class IEventsDestroy;
class IEventsDragDrop;
class IEventsGameObjectVisibilityChanged;
class IEventsSceneManager;
class Material;
class MeshRenderer;
class Path;
class Scene;
class UIFocusable;
class UIImageRenderer;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class IEventsScenePlayer;

class UISceneEditContainer
    : public UISceneContainer,
      public EventListener<IEventsGameObjectVisibilityChanged>,
      public EventListener<IEventsScenePlayer>,
      public EventListener<IEventsSceneManager>,
      public EventListener<IEventsDragDrop>
{
    GAMEOBJECT_EDITOR_WITHOUT_CLASS_ID(UISceneEditContainer);

public:
    UISceneEditContainer();
    virtual ~UISceneEditContainer() override;

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    static bool HasFocus();
    static bool IsMouseOver();
    static Vector2i GetMousePositionInOpenScene();

private:
    bool m_needToRenderPreviewImg = false;
    UIImageRenderer *p_cameraPreviewImg = nullptr;
    GBuffer *m_cameraPreviewGBuffer = nullptr;

    AH<Material> m_currentMaterialBeingDragged;
    GameObject *p_lastOveredGameObject = nullptr;
    MetaNode m_prevGameObjectMetaBeforeDraggingMaterial;
    Map<MeshRenderer *, AH<Material>> m_matDragMeshRenderersToPrevMaterials;

    void RenderCameraPreviewIfSelected();
    Camera *GetSceneCamera(Scene *scene) override;
    bool NeedsToRenderContainedScene(Scene *scene) override;
    void OnRenderContainedSceneBegin() override;
    void OnRenderContainedSceneFinished() override;

    // Material drag related
    GameObject *GetCurrentOveredGameObject() const;
    void ApplyDraggedMaterialToOveredGameObject();
    void RestoreDraggedMaterialToPreviousGameObjectOvered();

    // IEventsGameObjectVisibilityChanged
    void OnVisibilityChanged(GameObject *go) override;

    // IEventsFocus
    virtual UIEventResult OnUIEvent(UIFocusable *focusable,
                                    const UIEvent &event) override;
    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    // IEventsDragDrop
    virtual void OnDragStarted(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDragUpdate(
        EventEmitter<IEventsDragDrop> *dragDroppable) override;
    virtual void OnDrop(EventEmitter<IEventsDragDrop> *dragDroppable) override;

    // IEventsScenePlayer
    void OnPlayStateChanged(PlayState previousPlayState,
                            PlayState newPlayState) override;

    // IEventsSceneManager
    void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;
    static UISceneEditContainer *GetActive();

    friend class UISceneToolbar;
};
}

#endif  // UISCENEEDITCONTAINER_H
