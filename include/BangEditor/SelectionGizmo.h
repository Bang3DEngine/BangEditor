#ifndef SELECTIONGIZMO_H
#define SELECTIONGIZMO_H

#include "Bang/IDestroyListener.h"

#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SelectionGizmo : public IDestroyListener
{
public:
    SelectionGizmo() = default;
    virtual ~SelectionGizmo() = default;

    virtual void SetReferencedGameObject(GameObject *referencedGameObject);
    GameObject *GetReferencedGameObject() const;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

protected:
    enum SelectionState { Idle, Over, Grabbed };

private:
    GameObject *p_referencedGameObject = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMO_H

