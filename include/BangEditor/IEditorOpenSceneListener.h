#ifndef IEDITOROPENSCENELISTENER_H
#define IEDITOROPENSCENELISTENER_H

#include "Bang/IEventListener.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Scene;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class IEditorOpenSceneListener : public virtual IEventListener
{
public:
    virtual void OnOpenScene(Scene *scene) = 0;
};

NAMESPACE_BANG_EDITOR_END

#endif // IEDITOROPENSCENELISTENER
