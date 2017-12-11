#include "BangEditor/TransformGizmo.h"

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
}
