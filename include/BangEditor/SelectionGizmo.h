#ifndef SELECTIONGIZMO_H
#define SELECTIONGIZMO_H

#include "Bang/BangDefines.h"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsDestroy.h"
#include "Bang/MetaNode.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class IEventsDestroy;
class Camera;
class Object;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class SelectionGizmo : public GameObject, public EventListener<IEventsDestroy>

{
public:
    enum class SelectionState
    {
        IDLE,
        OVER,
        GRABBED
    };

    SelectionGizmo() = default;
    virtual ~SelectionGizmo() override = default;

    // GameObject
    virtual void Update() override;

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

protected:
    virtual GameObject *GetSelectionGameObject() const;

private:
    GameObject *p_referencedGameObject = nullptr;

    SelectionState m_selectionState = SelectionState::IDLE;
    bool m_grabHasJustChanged = false;
};
}

#endif  // SELECTIONGIZMO_H
