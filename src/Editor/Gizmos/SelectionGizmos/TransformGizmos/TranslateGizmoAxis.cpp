#include "BangEditor/TranslateGizmoAxis.h"

#include "Bang/Array.tcc"
#include "Bang/Camera.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/Geometry.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Plane.h"
#include "Bang/Ray.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Transform.h"
#include "Bang/Vector3.h"
#include "BangEditor/Selection.h"

namespace Bang
{
class Color;
}

using namespace Bang;
using namespace BangEditor;

TranslateGizmoAxis::TranslateGizmoAxis()
{
    SetName("TranslateGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    p_lineRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    for (MeshRenderer *mr : p_arrowCap->GetComponents<MeshRenderer>())
    {
        mr->SetMaterial(MaterialFactory::GetGizmosUnLightedOverlay().Get());
        mr->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);
        mr->GetMaterial()->SetReceivesLighting(false);
    }
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    p_meshRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_meshRenderer->SetMesh(MeshFactory::GetCone().Get());
    p_meshRenderer->SetCastsShadows(false);
    p_meshRenderer->SetReceivesShadows(false);
    p_arrowCap->GetTransform()->SetLocalScale(Vector3(0.5f, 0.5f, 1.0f));

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    p_selectionRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_selectionRenderer->SetMesh(MeshFactory::GetCube().Get());
    p_selectionRenderer->SetCastsShadows(false);
    p_selectionRenderer->SetReceivesShadows(false);

    p_arrowCap->SetParent(this);
    p_selectionGo->SetParent(this);
}

TranslateGizmoAxis::~TranslateGizmoAxis()
{
}

void TranslateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (!GetReferencedGameObject() ||
        !GetReferencedGameObject()->GetTransform())
    {
        return;
    }

    if (IsBeingGrabbed())
    {
        // Move along axis.
        // First. Find the plane parallel to the axes, and which faces the
        // camera the most.
        Camera *cam = GetEditorCamera();
        Transform *camT = cam->GetGameObject()->GetTransform();
        Vector3 refGoCenter =
            GetReferencedGameObject()->GetTransform()->GetPosition();

        Plane plane;
        plane.SetNormal(Vector3::Cross(GetAxisVectorWorld(),
                                       Vector3::Cross(camT->GetForward(),
                                                      GetAxisVectorWorld()))
                            .Normalized());
        plane.SetPoint(refGoCenter);

        // Then cast a ray through the mouse position, and see where it
        // intersects
        // with this plane.
        Ray mouseRay =
            cam->FromViewportPointNDCToRay(Input::GetMousePositionNDC());

        bool intersected;
        Vector3 intersection;
        Geometry::IntersectRayPlane(
            mouseRay, plane, &intersected, &intersection);

        // Then, move the object to the intersection
        if (intersected)
        {
            GameObject *refGo = GetReferencedGameObject();
            Transform *refGoT = refGo->GetTransform();

            Vector3 mousePoint = (intersection - refGoCenter);
            mousePoint = mousePoint.ProjectedOnVector(GetAxisVectorWorld());

            if (GrabHasJustChanged())
            {
                m_startGrabPoint = mousePoint;
            }
            else
            {
                mousePoint -= m_startGrabPoint;

                if (Input::GetKey(Key::LSHIFT))
                {
                    constexpr float SnappingDist = 1.0f;
                    float dl = mousePoint.Length();
                    float snapDL =
                        Math::Round(dl / SnappingDist) * SnappingDist;
                    mousePoint = mousePoint.NormalizedSafe() * snapDL;
                }

                refGoT->Translate(mousePoint);
            }
        }
    }
}

void TranslateGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    p_selectionGo->SetEnabled(Selection::IsBeingRendered());
    TransformGizmoAxis::Render(renderPass, renderChildren);
}

void TranslateGizmoAxis::SetAxis(Axis3DExt axis)
{
    TransformGizmoAxis::SetAxis(axis);

    Vector3 axisFwd = GetAxisVectorLocal();
    p_lineRenderer->SetPoints({Vector3::Zero(), axisFwd});

    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);
    Vector3 axisUp = (axis != Axis3DExt::Y) ? Vector3::Up() : Vector3::Right();
    p_arrowCap->GetTransform()->SetLocalRotation(
        Quaternion::LookDirection(axisFwd, axisUp));

    constexpr float baseScale = 0.4f;
    p_selectionGo->GetTransform()->SetLocalScale(Vector3(baseScale) +
                                                 Vector3::Abs(axisFwd));
    p_selectionGo->GetTransform()->SetLocalPosition(axisFwd *
                                                    (1.0f + baseScale) * 0.5f);
}

GameObject *TranslateGizmoAxis::GetSelectionGameObject() const
{
    return p_selectionGo;
}

void TranslateGizmoAxis::SetColor(const Color &color)
{
    p_lineRenderer->GetMaterial()->SetAlbedoColor(color);
    p_meshRenderer->GetMaterial()->SetAlbedoColor(color);
}
