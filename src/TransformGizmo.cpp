#include "BangEditor/TransformGizmo.h"

#include "Bang/Input.h"
#include "Bang/Vector3.h"
#include "Bang/Transform.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/TranslateGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TransformGizmo::TransformGizmo()
{
    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();

    p_translateGizmo = GameObject::Create<TranslateGizmo>();
    p_translateGizmo->SetParent(this);
}

TransformGizmo::~TransformGizmo()
{
}

void TransformGizmo::Update()
{
    GameObject::Update();

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
            break;

        case TransformMode::Rotate:
            p_translateGizmo->SetEnabled(false);
            break;

        case TransformMode::Scale:
            p_translateGizmo->SetEnabled(false);
            break;
    }
}

void TransformGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_translateGizmo->SetReferencedGameObject(referencedGameObject);
}
