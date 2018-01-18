#ifndef SELECTIONGIZMO_H
#define SELECTIONGIZMO_H

#include "Bang/IDestroyListener.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Camera;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SelectionGizmo : public IDestroyListener
{
public:
    SelectionGizmo() = default;
    virtual ~SelectionGizmo() = default;

    virtual void SetReferencedGameObject(GameObject *referencedGameObject);
    GameObject *GetReferencedGameObject() const;

    float GetScaleFactor() const;
    Camera *GetEditorCamera() const;

    // IDestroyListener
    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

protected:
    enum SelectionState { Idle, Over, Grabbed };

private:
    GameObject *p_referencedGameObject = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMO_H

