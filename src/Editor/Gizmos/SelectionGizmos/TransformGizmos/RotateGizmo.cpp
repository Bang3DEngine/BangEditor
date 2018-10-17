#include "BangEditor/RotateGizmo.h"

#include <sys/types.h>

#include "Bang/Array.h"
#include "Bang/Array.tcc"
#include "Bang/Axis.h"
#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/LineRenderer.h"
#include "Bang/Material.h"
#include "Bang/MaterialFactory.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Transform.h"
#include "Bang/Vector.tcc"
#include "Bang/Vector3.h"
#include "BangEditor/RotateGizmoAxis.h"

using namespace Bang;
using namespace BangEditor;

RotateGizmo::RotateGizmo()
{
    AddComponent<Transform>();

    p_sphereGo = GameObjectFactory::CreateGameObject();
    p_sphereGo->SetName("Sphere");

    p_sphereRenderer = p_sphereGo->AddComponent<MeshRenderer>();
    p_sphereRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_sphereRenderer->SetMesh(MeshFactory::GetSphere().Get());
    p_sphereRenderer->GetMaterial()->SetAlbedoColor(Color(1, 1, 1, 0.25f));
    p_sphereRenderer->SetVisible(false);
    p_sphereGo->GetTransform()->SetLocalScale(0.97f);

    p_sphereBoundsGo = GameObjectFactory::CreateGameObject();
    p_sphereBoundsGo->SetName("SphereBounds");

    p_sphereBoundsRenderer = p_sphereBoundsGo->AddComponent<LineRenderer>();
    p_sphereBoundsRenderer->SetMaterial(
        MaterialFactory::GetGizmosUnLightedOverlay().Get());
    p_sphereBoundsRenderer->GetMaterial()->SetAlbedoColor(Color::Black);
    CreateSphereBoundsPoints();

    p_sphereGo->SetParent(this);
    p_sphereBoundsGo->SetParent(this);

    p_axisX = GameObject::Create<RotateGizmoAxis>();
    p_axisY = GameObject::Create<RotateGizmoAxis>();
    p_axisZ = GameObject::Create<RotateGizmoAxis>();

    p_axisX->SetAxis(Axis3DExt::X);
    p_axisY->SetAxis(Axis3DExt::Y);
    p_axisZ->SetAxis(Axis3DExt::Z);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

RotateGizmo::~RotateGizmo()
{
}

void RotateGizmo::Update()
{
    SelectionGizmo::Update();
}

void RotateGizmo::Render(RenderPass rp, bool renderChildren)
{
    // Update sphere so that it faces camera
    Camera *cam = GetEditorCamera();
    Transform *camT = cam->GetGameObject()->GetTransform();
    Vector3 camCenterDir =
        (GetTransform()->GetPosition() - camT->GetPosition());
    p_sphereBoundsGo->GetTransform()->SetRotation(
        Quaternion::LookDirection(camCenterDir));

    GameObject::Render(rp, renderChildren);  // Do Render
}

void RotateGizmo::CreateSphereBoundsPoints()
{
    constexpr int numSegments = 32;
    constexpr float angleStep = (Math::Pi * 2.0f) / numSegments;

    // Get circle points
    Array<Vector3> circlePoints;
    for (uint i = 0; i < numSegments + 1; ++i)
    {
        float angle = angleStep * i;
        Vector3 newPoint(Math::Sin(angle), Math::Cos(angle), 0.0f);
        circlePoints.PushBack(newPoint);
    }

    // Get renderer points
    Array<Vector3> rendererPoints;
    for (uint i = 0; i < circlePoints.Size() - 1; ++i)
    {
        rendererPoints.PushBack(circlePoints[i + 0]);
        rendererPoints.PushBack(circlePoints[i + 1]);
    }

    p_sphereBoundsRenderer->SetPoints(rendererPoints);
}

void RotateGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_axisX->SetReferencedGameObject(GetReferencedGameObject());
    p_axisY->SetReferencedGameObject(GetReferencedGameObject());
    p_axisZ->SetReferencedGameObject(GetReferencedGameObject());
}
