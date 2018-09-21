#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/GameObject.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsTransform.h"
#include "Bang/IEventsValueChanged.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneImage;
FORWARD class UISceneToolbar;

class UISceneContainer : public GameObject,
                         public EventListener<IEventsDestroy>,
                         public EventListener<IEventsValueChanged>,
                         public EventListener<IEventsTransform>
{
public:
    UISceneContainer();
    virtual ~UISceneContainer();

    void RenderIfNeeded();

    void SetScene(Scene *scene);

    Scene *GetContainedScene() const;
    AARect GetSceneImageAARectNDC() const;

    UISceneToolbar* GetSceneToolbar() const;
    UISceneImage* GetSceneImage() const;

protected:
    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

private:
    Scene *p_containedScene = nullptr;

    UISceneToolbar *p_sceneToolbar = nullptr;
    UISceneImage *p_sceneImage = nullptr;

    GameObject *p_noCameraOverlay = nullptr;

    virtual Camera* GetSceneCamera(Scene *scene) = 0;
    virtual bool NeedsToRenderScene(Scene *scene) = 0;
    virtual void OnRenderNeededSceneFinished();

    // IEventsTransform
    void OnTransformChanged() override;

    // IEventsValueChanged
    void OnValueChanged(EventEmitter<IEventsValueChanged> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENECONTAINER_H

