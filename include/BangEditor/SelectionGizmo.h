#ifndef SELECTIONGIZMO_H
#define SELECTIONGIZMO_H

#include "Bang/MetaNode.h"
#include "Bang/GameObject.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Camera;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SelectionGizmo : public GameObject
{
public:
    enum class SelectionState { IDLE, OVER, GRABBED };

    SelectionGizmo() = default;
    virtual ~SelectionGizmo() = default;

    // GameObject
    void Update() override;

    virtual void SetReferencedGameObject(GameObject *referencedGameObject);
    GameObject *GetReferencedGameObject() const;

    float GetScaleFactor() const;
    Camera *GetEditorCamera() const;

    SelectionState GetSelectionState() const;
    bool IsBeingGrabbed() const;
    bool GrabHasJustChanged() const;

    virtual void OnGrabBegin();
    virtual void OnGrabEnd();

    // IEventsObject
    void OnDisabled(Object *object) override;

    // IEventsDestroy
    virtual void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;

private:
    GameObject *p_referencedGameObject = nullptr;

    SelectionState m_selectionState = SelectionState::IDLE;
    bool m_grabHasJustChanged = false;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMO_H

