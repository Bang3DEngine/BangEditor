#include "BangEditor/TranslateGizmo.h"

#include "BangMath/Axis.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Transform.h"
#include "BangEditor/TranslateGizmoAxis.h"

using namespace Bang;
using namespace BangEditor;

TranslateGizmo::TranslateGizmo()
{
    AddComponent<Transform>();

    p_axisX = new TranslateGizmoAxis();
    p_axisY = new TranslateGizmoAxis();
    p_axisZ = new TranslateGizmoAxis();

    p_axisX->SetAxis(Axis3DExt::X);
    p_axisY->SetAxis(Axis3DExt::Y);
    p_axisZ->SetAxis(Axis3DExt::Z);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

TranslateGizmo::~TranslateGizmo()
{
}

void TranslateGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_axisX->SetReferencedGameObject(GetReferencedGameObject());
    p_axisY->SetReferencedGameObject(GetReferencedGameObject());
    p_axisZ->SetReferencedGameObject(GetReferencedGameObject());
}
