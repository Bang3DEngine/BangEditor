#include "BangEditor/ScaleGizmo.h"

#include "Bang/Axis.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Transform.h"
#include "BangEditor/ScaleGizmoAxis.h"

using namespace Bang;
using namespace BangEditor;

ScaleGizmo::ScaleGizmo()
{
    AddComponent<Transform>();

    p_axisX = GameObject::Create<ScaleGizmoAxis>();
    p_axisY = GameObject::Create<ScaleGizmoAxis>();
    p_axisZ = GameObject::Create<ScaleGizmoAxis>();
    p_axisXYZ = GameObject::Create<ScaleGizmoAxis>();

    p_axisX->SetAxis(Axis3DExt::X);
    p_axisY->SetAxis(Axis3DExt::Y);
    p_axisZ->SetAxis(Axis3DExt::Z);
    p_axisXYZ->SetAxis(Axis3DExt::XYZ);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
    p_axisXYZ->SetParent(this);
}

ScaleGizmo::~ScaleGizmo()
{
}

void ScaleGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_axisX->SetReferencedGameObject(GetReferencedGameObject());
    p_axisY->SetReferencedGameObject(GetReferencedGameObject());
    p_axisZ->SetReferencedGameObject(GetReferencedGameObject());
    p_axisXYZ->SetReferencedGameObject(GetReferencedGameObject());
}
