#include "BangEditor/EditorMeshFactoryUITabScene.h"

#include "Bang/Camera.h"
#include "Bang/DirectionalLight.h"
#include "Bang/Extensions.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Model.h"
#include "Bang/RectTransform.h"
#include "Bang/Resources.h"
#include "Bang/Scene.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UIVerticalLayout.h"
#include "BangEditor/Explorer.h"

using namespace Bang;
using namespace BangEditor;

EditorMeshFactoryUITabScene::EditorMeshFactoryUITabScene()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

    // UIHorizontalLayout *hl = AddComponent<UIHorizontalLayout>();
    // hl->SetPaddings(15);

    p_scene = GameObjectFactory::CreateScene();

    GameObject *meshContainerGo = GameObjectFactory::CreateGameObject();
    p_meshContainerRenderer = meshContainerGo->AddComponent<MeshRenderer>();
    meshContainerGo->SetParent(p_scene);

    GameObject *sceneCamGo = GameObjectFactory::CreateGameObject();
    p_sceneCamera = GameObjectFactory::CreateDefaultCameraInto(sceneCamGo);
    sceneCamGo->SetParent(p_scene);

    p_scene->SetCamera(p_sceneCamera);

    GameObject *lightGo = GameObjectFactory::CreateGameObject();
    DirectionalLight *dirLight = lightGo->AddComponent<DirectionalLight>();
    lightGo->GetTransform()->LookInDirection(Vector3::Down());
    lightGo->SetParent(p_scene);

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg = sceneImgGo->AddComponent<UIImageRenderer>();
    p_sceneImg->SetMode(UIImageRenderer::Mode::TEXTURE);
    sceneImgGo->SetParent(this);
}

EditorMeshFactoryUITabScene::~EditorMeshFactoryUITabScene()
{
}

void EditorMeshFactoryUITabScene::Update()
{
    GameObject::Update();

    // Camera setup
    {
        Vector2 mouseCurrentAxisMovement = Input::GetMouseAxis();
        m_currentCameraRotAngles += mouseCurrentAxisMovement * 10.0f;

        m_currentCameraZoom +=
            (Input::GetMouseWheel().y * 0.1f) * m_currentCameraZoom;

        Transform *camTR = p_sceneCamera->GetGameObject()->GetTransform();
        Sphere goSphere =
            p_meshContainerRenderer->GetGameObject()->GetBoundingSphere();
        float halfFov = Math::DegToRad(p_sceneCamera->GetFovDegrees() / 2.0f);
        float camDist = goSphere.GetRadius() / Math::Tan(halfFov) * 1.1f;
        camDist *= m_currentCameraZoom;
        Vector3 camDir =
            (Quaternion::AngleAxis(Math::DegToRad(-m_currentCameraRotAngles.x),
                                   Vector3::Up()) *
             Quaternion::AngleAxis(Math::DegToRad(m_currentCameraRotAngles.y),
                                   Vector3::Right()) *
             Vector3(0, 0, 1))
                .Normalized();
        camTR->SetPosition(goSphere.GetCenter() + camDir * camDist);
        camTR->LookAt(goSphere.GetCenter());
        p_sceneCamera->SetZFar((camDist + goSphere.GetRadius() * 2.0f) * 1.2f);
    }

    // Render camera
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Push(GL::Pushable::VIEWPORT);

    Vector2i sceneImgSize(p_sceneImg->GetGameObject()
                              ->GetRectTransform()
                              ->GetViewportAARect()
                              .GetSize());

    p_sceneCamera->SetRenderSize(sceneImgSize);
    GL::SetViewport(0, 0, sceneImgSize.x, sceneImgSize.y);

    p_lastSelectedMesh.Set(GetExplorerSelectedMesh().Get());
    p_meshContainerRenderer->SetMesh(p_lastSelectedMesh.Get());
    GEngine::GetInstance()->RenderToGBuffer(p_scene, p_sceneCamera);

    p_sceneImg->SetImageTexture(
        p_sceneCamera->GetGBuffer()->GetLastDrawnColorTexture());

    GL::Pop(GL::Pushable::VIEWPORT);
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
}

void EditorMeshFactoryUITabScene::ResetCamera()
{
    m_currentCameraZoom = 1.0f;
    m_currentCameraRotAngles = Vector2::Zero();
}

RH<Mesh> EditorMeshFactoryUITabScene::GetExplorerSelectedMesh() const
{
    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(Extensions::GetMeshExtension()))
        {
            RH<Mesh> mesh = Resources::Load<Mesh>(selectedPath);
            return mesh;
        }
        else if (selectedPath.HasExtension(Extensions::GetModelExtensions()))
        {
            RH<Model> model = Resources::Load<Model>(selectedPath);
            if (model.Get())
            {
                if (model.Get()->GetMeshes().Size() >= 1)
                {
                    return model.Get()->GetMeshes().Front();
                }
            }
        }
    }
    return RH<Mesh>();
}
