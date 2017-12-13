#include "BangEditor/RotateGizmo.h"

#include "Bang/Transform.h"

#include "BangEditor/RotateGizmoAxis.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

RotateGizmo::RotateGizmo()
{
    AddComponent<Transform>();

    p_axisX = GameObject::Create<RotateGizmoAxis>();
    p_axisY = GameObject::Create<RotateGizmoAxis>();
    p_axisZ = GameObject::Create<RotateGizmoAxis>();

    p_axisX->SetAxis(Axis3D::X);
    p_axisY->SetAxis(Axis3D::Y);
    p_axisZ->SetAxis(Axis3D::Z);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

RotateGizmo::~RotateGizmo()
{

}

void RotateGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_axisX->SetReferencedGameObject( GetReferencedGameObject() );
    p_axisY->SetReferencedGameObject( GetReferencedGameObject() );
    p_axisZ->SetReferencedGameObject( GetReferencedGameObject() );
}
