#include "BangEditor/SelectionGizmo.h"

#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/Selection.h"
#include "Bang/GameObject.h"

#include "BangEditor/EditorCamera.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void SelectionGizmo::Update()
{
    GameObject::Update();

    // Update selectionState
    Camera *editorCamera = GetEditorCamera();

    bool isMouseOver = false;
    GameObject *overedGameObject = Selection::GetOveredGameObject(editorCamera);
    if (overedGameObject)
    {
        isMouseOver = (overedGameObject == this);
        isMouseOver = isMouseOver || (overedGameObject->IsChildOf(this, true));
    }

    bool prevGrab = IsBeingGrabbed();
    if ( Input::GetMouseButtonDown(MouseButton::Left) )
    {
        if (isMouseOver) { m_selectionState = SelectionState::Grabbed; }
    }
    else if ( !Input::GetMouseButton(MouseButton::Left) )
    {
        m_selectionState = isMouseOver ? SelectionState::Over :
                                         SelectionState::Idle;
    }
    m_grabHasJustChanged = (IsBeingGrabbed() != prevGrab);
}

void SelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    if (GetReferencedGameObject() != referencedGameObject)
    {
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->
                    EventEmitter<IDestroyListener>::UnRegisterListener(this);
        }

        p_referencedGameObject = referencedGameObject;
        if (GetReferencedGameObject())
        {
            GetReferencedGameObject()->
                    EventEmitter<IDestroyListener>::RegisterListener(this);
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
    Camera *editorCamera = editorCameraGo ? editorCameraGo->GetCamera() : nullptr;
    return editorCamera;
}

SelectionGizmo::SelectionState SelectionGizmo::GetSelectionState() const
{
    return m_selectionState;
}

bool SelectionGizmo::IsBeingGrabbed() const
{
    return GetSelectionState() == SelectionState::Grabbed;
}

bool SelectionGizmo::GrabHasJustChanged() const
{
    return m_grabHasJustChanged;
}

void SelectionGizmo::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    ASSERT( !GetReferencedGameObject() || object == GetReferencedGameObject() );
    SetReferencedGameObject(nullptr);
}

void SelectionGizmo::OnDisabled()
{
    GameObject::OnDisabled();
    m_selectionState = SelectionState::Idle;
    m_grabHasJustChanged = false;
}
