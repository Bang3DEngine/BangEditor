#include "BangEditor/ComponentsSelectionGizmo.h"

#include "Bang/GL.h"
#include "Bang/Mesh.h"
#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Gizmos.h"
#include "Bang/Transform.h"
#include "Bang/Resources.h"
#include "Bang/MeshFactory.h"
#include "Bang/SceneManager.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentsSelectionGizmo::ComponentsSelectionGizmo()
{
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
}

ComponentsSelectionGizmo::~ComponentsSelectionGizmo()
{
}

void ComponentsSelectionGizmo::Update()
{
    GameObject::Update();
}

void ComponentsSelectionGizmo::RenderGizmos()
{
    GameObject::RenderGizmos();

    for (Component *comp : GetReferencedGameObject()->GetComponents())
    {
        if (Camera *cam = DCAST<Camera*>(comp)) { RenderCameraGizmo(cam); }
    }
}

void ComponentsSelectionGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);
}

void ComponentsSelectionGizmo::RenderCameraGizmo(Camera *cam)
{
    static RH<Mesh> cameraMesh = MeshFactory::GetCamera();
    Camera *sceneCam = SceneManager::GetActiveScene()->GetCamera();
    Transform *sceneCamTransform = sceneCam->GetGameObject()->GetTransform();
    Transform *camTransform = cam->GetGameObject()->GetTransform();
    float distScale = Vector3::Distance(sceneCamTransform->GetPosition(),
                                        camTransform->GetPosition());

    Gizmos::SetReceivesLighting(true);
    Gizmos::SetPosition(camTransform->GetPosition());
    Gizmos::SetRotation(camTransform->GetRotation());
    Gizmos::SetScale(Vector3::One * 0.02f * distScale);
    Gizmos::SetColor(Color::White);
    Gizmos::RenderCustomMesh(cameraMesh.Get());

    Gizmos::Reset();
    Gizmos::SetColor(Color::Red);
    Gizmos::SetReceivesLighting(false);

    if (cam->GetProjectionMode() == Camera::ProjectionMode::Perspective)
    {
        Gizmos::RenderFrustum(camTransform->GetForward(),
                              camTransform->GetUp(),
                              camTransform->GetPosition(),
                              cam->GetZNear(),
                              cam->GetZFar(),
                              cam->GetFovDegrees(),
                              GL::GetViewportAspectRatio() );
    }
    else
    {
        AABox orthoBox;
        Vector3 pos = camTransform->GetPosition();
        Vector2 orthoSize = Vector2(cam->GetOrthoWidth(), cam->GetOrthoHeight());
        orthoBox.SetMin(pos + Vector3(-orthoSize.x, -orthoSize.y, -cam->GetZNear()));
        orthoBox.SetMax(pos + Vector3( orthoSize.x,  orthoSize.y, -cam->GetZFar()));
        Gizmos::SetRotation(camTransform->GetRotation());
        Gizmos::RenderSimpleBox(orthoBox);
    }
}
