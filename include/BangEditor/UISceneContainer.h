#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/GameObject.h"
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
                         public IValueChangedListener
{
public:
    UISceneContainer();
    virtual ~UISceneContainer();

    void SetScene(Scene *scene);

    Scene *GetContainedScene() const;
    Rect GetSceneImageRectNDC() const;

protected:
    UISceneToolbar* GetSceneToolbar() const;

private:
    Scene *p_scene = nullptr;

    UISceneToolbar *p_sceneToolbar = nullptr;
    UISceneImage *p_sceneImage = nullptr;

    virtual Camera* GetSceneCamera(Scene *scene) = 0;

    // IValueChangedListener
    void OnValueChanged(Object *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // UISCENECONTAINER_H

