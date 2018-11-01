#include "BangEditor/SelectionGizmo.h"

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Assert.h"
#include "Bang/Camera.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/Input.h"
#include "Bang/MouseButton.h"
#include "Bang/Transform.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/Selection.h"

namespace Bang
{
class IEventsDestroy;
class Object;
}  // namespace Bang

using namespace Bang;
using namespace BangEditor;

void SelectionGizmo::Update()
{
    GameObject::Update();

    // Update selectionState
    bool isMouseOver = false;
    GameObject *overedGameObject = Selection::GetOveredGameObject();
    if (overedGameObject)
    {
        isMouseOver = (overedGameObject == this);
        isMouseOver = isMouseOver || (overedGameObject->IsChildOf(this, true));
    }

    bool prevGrab = IsBeingGrabbed();
    if (Input::GetMouseButtonDown(MouseButton::LEFT))
    {
        if (isMouseOver)
        {
            m_selectionState = SelectionState::GRABBED;
        }
    }
    else if (!Input::GetMouseButton(MouseButton::LEFT))
    {
        m_selectionState =
            isMouseOver ? SelectionState::OVER : SelectionState::IDLE;
    }
    m_grabHasJustChanged = (IsBeingGrabbed() != prevGrab);

    // Grab event
    if (GrabHasJustChanged())
    {
        if (IsBeingGrabbed())
        {
            OnGrabBegin();
        }
        else
        {
            OnGrabEnd();
        }
    }
}

void SelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (GetReferencedGameObject() != referencedGameObject)
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()
                ->EventEmitter<IEventsDestroy>::UnRegisterListener(this);
        }

        p_referencedGameObject = referencedGameObject;
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()
                ->EventEmitter<IEventsDestroy>::RegisterListener(this);
        }
    }
}

GameObject *SelectionGizmo::GetReferencedGameObject() const
{
    return p_referencedGameObject;
}

float SelectionGizmo::GetScaleFactor() const
{
    GameObject *refGo = GetReferencedGameObject();
    Transform *camT = GetEditorCamera()->GetGameObject()->GetTransform();
    float camDist = Vector3::Distance(refGo->GetTransform()->GetPosition(),
                                      camT->GetPosition());
    return camDist;
}

Camera *SelectionGizmo::GetEditorCamera() const
{
    EditorCamera *editorCameraGo = EditorCamera::GetInstance();
    Camera *editorCamera =
        editorCameraGo ? editorCameraGo->GetCamera() : nullptr;
    return editorCamera;
}

SelectionGizmo::SelectionState SelectionGizmo::GetSelectionState() const
{
    return m_selectionState;
}

bool SelectionGizmo::IsBeingGrabbed() const
{
    return GetSelectionState() == SelectionState::GRABBED;
}

bool SelectionGizmo::GrabHasJustChanged() const
{
    return m_grabHasJustChanged;
}

void SelectionGizmo::OnGrabBegin()
{
    // Empty
}

void SelectionGizmo::OnGrabEnd()
{
    // Empty
}

void SelectionGizmo::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
    GameObject::OnDestroyed(object);

    ASSERT(!GetReferencedGameObject() || object == GetReferencedGameObject());
    SetReferencedGameObject(nullptr);
}

void SelectionGizmo::OnDisabled(Object *object)
{
    GameObject::OnDisabled(object);
    m_selectionState = SelectionState::IDLE;
    m_grabHasJustChanged = false;
}
