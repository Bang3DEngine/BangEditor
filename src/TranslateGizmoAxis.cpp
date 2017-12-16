#include "BangEditor/TranslateGizmoAxis.h"

#include "Bang/Ray.h"
#include "Bang/Mesh.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/Selection.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/AxisFunctions.h"
#include "Bang/DebugRenderer.h"
#include "Bang/MaterialFactory.h"
#include "Bang/RendererFactory.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TranslateGizmoAxis::TranslateGizmoAxis()
{
    SetName("TranslateGizmoAxis");

    p_lineRenderer = AddComponent<LineRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_lineRenderer);

    p_arrowCap = GameObjectFactory::CreateGameObject(true);
    p_arrowCap->SetName("ArrowCap");

    p_meshRenderer = p_arrowCap->AddComponent<MeshRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_meshRenderer);
    p_meshRenderer->SetMesh( MeshFactory::GetCone().Get() );
    p_arrowCap->GetTransform()->SetLocalScale( Vector3(0.5f, 0.5f, 1.0f) );

    p_selectionGo = GameObjectFactory::CreateGameObject(true);
    p_selectionGo->SetName("AxisSelection");

    p_selectionRenderer = p_selectionGo->AddComponent<MeshRenderer>();
    RendererFactory::ConvertToGizmoRenderer(p_meshRenderer);
    p_selectionRenderer->SetMesh( MeshFactory::GetCube().Get() );

    p_arrowCap->SetParent(this);
    p_selectionGo->SetParent(this);
}

TranslateGizmoAxis::~TranslateGizmoAxis()
{
}

void TranslateGizmoAxis::Update()
{
    TransformGizmoAxis::Update();

    if (IsBeingGrabbed())
    {
        // Move along axis.
        // First. Find the plane parallel to the axes, and which faces the
        // camera the most.
        Camera *cam = Camera::GetActive();
        Transform *camT = cam->GetGameObject()->GetTransform();
        Vector3 planeNormal = Vector3::Cross( GetAxisVectorWorld(),
                                Vector3::Cross(camT->GetForward(),
                                               GetAxisVectorWorld())).Normalized();
        Vector3 refGoCenter = GetReferencedGameObject()->GetTransform()->GetPosition();

        // Then cast a ray through the mouse position, and see where it intersects
        // with this plane.
        Ray mouseRay = Camera::GetActive()->
                       FromViewportPointNDCToRay( Input::GetMousePositionNDC() );

        bool intersected;
        Vector3 intersection;
        mouseRay.GetIntersectionWithPlane(refGoCenter, planeNormal,
                                          &intersected, &intersection);

        // Then, move the object to the intersection
        if (intersected)
        {
            GameObject *refGo = GetReferencedGameObject();
            Transform *refGoT = refGo->GetTransform();

            Vector3 mousePoint = (intersection - refGoCenter);
            mousePoint = mousePoint.ProjectedOnVector(GetAxisVectorWorld());

            if (GrabHasJustChanged()) { m_startGrabPoint = mousePoint; }
            else
            {
                mousePoint -= m_startGrabPoint;

                if (Input::GetKey(Key::LShift))
                {
                    constexpr float SnappingDist = 1.0f;
                    float dl = mousePoint.Length();
                    float snapDL = Math::Round(dl / SnappingDist) * SnappingDist;
                    mousePoint = mousePoint.NormalizedSafe() * snapDL;
                }

                refGoT->Translate(mousePoint);
            }
        }
    }
}

void TranslateGizmoAxis::Render(RenderPass renderPass, bool renderChildren)
{
    bool selection = GL::IsBound( GEngine::GetActiveSelectionFramebuffer() );
    p_selectionGo->SetEnabled(selection);
    TransformGizmoAxis::Render(renderPass, renderChildren);
}

void TranslateGizmoAxis::SetAxis(Axis3D axis)
{
    TransformGizmoAxis::SetAxis(axis);

    Vector3 axisFwd = GetAxisVectorLocal();
    p_lineRenderer->SetPoints( {Vector3::Zero, axisFwd} );

    p_arrowCap->GetTransform()->SetLocalPosition(axisFwd);
    Vector3 axisUp = (axis != Axis3D::Y) ? Vector3::Up : Vector3::Right;
    p_arrowCap->GetTransform()->SetLocalRotation(
           Quaternion::LookDirection(axisFwd, axisUp) );

    constexpr float baseScale = 0.4f;
    p_selectionGo->GetTransform()->SetLocalScale( Vector3(baseScale) +
                                                  Vector3::Abs(axisFwd));
    p_selectionGo->GetTransform()->SetLocalPosition( axisFwd *
                                                     (1.0f + baseScale) * 0.5f);
}

void TranslateGizmoAxis::SetColor(const Color &color)
{
    p_lineRenderer->GetMaterial()->SetDiffuseColor(color);
    p_meshRenderer->GetMaterial()->SetDiffuseColor(color);
}

