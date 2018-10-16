#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsFocus.h"
#include "Bang/IEventsTransform.h"
#include "Bang/IEventsValueChanged.h"
#include "Bang/RenderPass.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class Camera;
class IEventsDestroy;
class IEventsTransform;
class IEventsValueChanged;
class Scene;
template <class > class EventEmitter;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD class UIFocusable;
FORWARD class UIImageRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneImage;
FORWARD class UISceneToolbar;
FORWARD class UISceneToolbarDown;

class UISceneContainer : public GameObject,
                         public EventListener<IEventsFocus>,
                         public EventListener<IEventsValueChanged>,
                         public EventListener<IEventsTransform>
{
public:
    UISceneContainer();
    virtual ~UISceneContainer();

    // GameObject
    void BeforeChildrenRender(RenderPass rp) override;

    void RenderContainedSceneIfNeeded();

    void SetScene(Scene *scene);

    Scene *GetContainedScene() const;
    AARect GetSceneImageAARectNDC() const;

    UISceneToolbar* GetSceneToolbar() const;
    UISceneImage* GetSceneImage() const;
    UIFocusable* GetFocusable() const;
    UISceneToolbarDown *GetSceneToolbarDown() const;

protected:
    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

    // IEventsFocus
    UIEventResult OnUIEvent(UIFocusable *focusable, const UIEvent &event) override;

private:
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_border = nullptr;

    Scene *p_containedScene = nullptr;

    UISceneToolbarDown *p_sceneToolbarDown = nullptr;
    UISceneToolbar *p_sceneToolbar = nullptr;
    UISceneImage *p_sceneImage = nullptr;

    GameObject *p_noCameraOverlay = nullptr;

    virtual Camera* GetSceneCamera(Scene *scene) = 0;
    virtual bool NeedsToRenderContainedScene(Scene *scene) = 0;
    virtual void OnRenderContainedSceneBegin();
    virtual void OnRenderContainedSceneFinished();

    // IEventsTransform
    void OnTransformChanged() override;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENECONTAINER_H

