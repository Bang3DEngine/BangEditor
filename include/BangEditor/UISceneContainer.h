#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/GameObject.h"
#include "Bang/EventListener.h"
#include "Bang/ITransformListener.h"
#include "Bang/IValueChangedListener.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UISceneImage;
FORWARD class UISceneToolbar;

class UISceneContainer : public GameObject,
                         public EventListener<IDestroyListener>,
                         public EventListener<IValueChangedListener>,
                         public EventListener<ITransformListener>
{
public:
    UISceneContainer();
    virtual ~UISceneContainer();

    void RenderIfNeeded();

    void SetScene(Scene *scene);

    Scene *GetContainedScene() const;
    AARect GetSceneImageRectNDC() const;

protected:
    UISceneToolbar* GetSceneToolbar() const;
    UISceneImage* GetSceneImage() const;

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

private:
    Scene *p_containedScene = nullptr;

    UISceneToolbar *p_sceneToolbar = nullptr;
    UISceneImage *p_sceneImage = nullptr;

    virtual Camera* GetSceneCamera(Scene *scene) = 0;
    virtual bool NeedsToRenderScene(Scene *scene) = 0;
    virtual void OnRenderNeededSceneFinished();

    // ITransformListener
    void OnTransformChanged() override;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENECONTAINER_H

