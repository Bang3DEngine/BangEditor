#include "BangEditor/TransformGizmo.h"

#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Vector3.h"
#include "Bang/Transform.h"

#include "BangEditor/ScaleGizmo.h"
#include "BangEditor/RotateGizmo.h"
#include "BangEditor/TranslateGizmo.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TransformGizmo::TransformGizmo()
{
    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DontSerialize);
    GetHideFlags().SetOn(HideFlag::DontClone);

    p_translateGizmo = GameObject::Create<TranslateGizmo>();
    p_rotateGizmo    = GameObject::Create<RotateGizmo>();
    p_scaleGizmo     = GameObject::Create<ScaleGizmo>();

    p_translateGizmo->SetParent(this);
    p_rotateGizmo->SetParent(this);
    p_scaleGizmo->SetParent(this);

    p_translateGizmo->SetEnabled(false);
    p_rotateGizmo->SetEnabled(false);
    p_scaleGizmo->SetEnabled(false);
}

TransformGizmo::~TransformGizmo()
{
}

void TransformGizmo::Update()
{
    GameObject::Update();

    GameObject *refGo = GetReferencedGameObject();
    if (!refGo || !refGo->GetTransform()) { return; }

    GetTransform()->SetPosition( refGo->GetTransform()->GetPosition() );
    GetTransform()->SetRotation( refGo->GetTransform()->GetRotation() );

    GetTransform()->SetScale( GetScaleFactor() );

    if      (Input::GetKeyDown(Key::W)) { m_transformMode = TransformMode::Translate; }
    else if (Input::GetKeyDown(Key::E)) { m_transformMode = TransformMode::Rotate; }
    else if (Input::GetKeyDown(Key::R)) { m_transformMode = TransformMode::Scale; }

    switch (m_transformMode)
    {
        case TransformMode::Translate:
            p_translateGizmo->SetEnabled(true);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(false);
            break;

        case TransformMode::Rotate:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(true);
            p_scaleGizmo->SetEnabled(false);
            break;

        case TransformMode::Scale:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(true);
            break;
    }
}

void TransformGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_translateGizmo->SetReferencedGameObject(referencedGameObject);
    p_rotateGizmo->SetReferencedGameObject(referencedGameObject);
    p_scaleGizmo->SetReferencedGameObject(referencedGameObject);
}

float TransformGizmo::GetScaleFactor() const
{
    GameObject *refGo = GetReferencedGameObject();
    Transform *camT = Camera::GetActive()->GetGameObject()->GetTransform();
    float camDist = Vector3::Distance(refGo->GetTransform()->GetPosition(),
                                      camT->GetPosition());
    return 0.15f * camDist;
}
