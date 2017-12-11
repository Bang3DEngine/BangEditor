#include "BangEditor/TranslateGizmoAxis.h"

#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/LineRenderer.h"
#include "Bang/AxisFunctions.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TranslateGizmoAxis::TranslateGizmoAxis()
{
    SetName("TranslateGizmo");

    AddComponent<Transform>();

    LineRenderer *lr = AddComponent<LineRenderer>();
    lr->SetRenderPass(RenderPass::Gizmos);
    lr->SetLineWidth(3.0f);
}

TranslateGizmoAxis::~TranslateGizmoAxis()
{
}

void TranslateGizmoAxis::SetAxis(Axis3D axis)
{
    if (axis != GetAxis())
    {
        m_axis = axis;

        LineRenderer *lr = GetComponent<LineRenderer>();
        lr->GetMaterial()->SetDiffuseColor( GetColorFromAxis(axis) );
        lr->SetPoints( {Vector3::Zero, GetVectorFromAxis(axis)} );
    }
}
Axis3D TranslateGizmoAxis::GetAxis() const { return m_axis; }
Vector3 TranslateGizmoAxis::GetAxisVector() const
{
    return GetVectorFromAxis( GetAxis() );
}
