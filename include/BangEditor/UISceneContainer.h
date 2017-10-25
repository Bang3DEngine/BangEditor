#ifndef UISCENECONTAINER_H
#define UISCENECONTAINER_H

#include "Bang/Rect.h"
#include "Bang/RenderPass.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Camera;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class UISceneContainer : public UIVerticalLayout
{
    COMPONENT(UISceneContainer)

public:
    UISceneContainer();

    virtual ~UISceneContainer();

private:
};

NAMESPACE_BANG_EDITOR_END

#endif // SUBSCENE_H
