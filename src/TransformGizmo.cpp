#include "BangEditor/TransformGizmo.h"

#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Vector3.h"
#include "Bang/Transform.h"

#include "BangEditor/ScaleGizmo.h"
#include "BangEditor/RotateGizmo.h"
#include "BangEditor/TranslateGizmo.h"
#include "BangEditor/HideInHierarchy.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TransformGizmo::TransformGizmo()
{
    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();

    p_translateGizmo = GameObject::Create<TranslateGizmo>();
    p_rotateGizmo    = GameObject::Create<RotateGizmo>();
    p_scaleGizmo     = GameObject::Create<ScaleGizmo>();

    p_translateGizmo->SetParent(this);
    p_rotateGizmo->SetParent(this);
    p_scaleGizmo->SetParent(this);
}

TransformGizmo::~TransformGizmo()
{
}

void TransformGizmo::Update()
{
    GameObject::Update();

    // Transform *camT = Camera::GetActive()->GetGameObject()->GetTransform();
    // float camDist = camT->GetPosition();
    // GetTransform()->SetScale();

    GameObject *refGo = GetReferencedGameObject();
    GetTransform()->SetPosition( refGo->GetTransform()->GetPosition() );
    GetTransform()->SetRotation( refGo->GetTransform()->GetRotation() );

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
