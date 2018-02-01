#ifndef SELECTIONGIZMO_H
#define SELECTIONGIZMO_H

#include "Bang/GameObject.h"
#include "Bang/IDestroyListener.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Camera;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SelectionGizmo : public GameObject,
                       public IDestroyListener
{
public:
    enum class SelectionState { Idle, Over, Grabbed };

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

    // IObjectListener
    void OnDisabled() override;

    // IDestroyListener
    virtual void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

private:
    GameObject *p_referencedGameObject = nullptr;

    SelectionState m_selectionState = SelectionState::Idle;
    bool m_grabHasJustChanged = false;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMO_H

