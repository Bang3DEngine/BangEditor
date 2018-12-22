#include "BangEditor/EditorMeshFactoryUITabScene.h"

#include "Bang/BoxCollider.h"
#include "Bang/Camera.h"
#include "Bang/DebugRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/Extensions.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/Geometry.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshCollider.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Model.h"
#include "Bang/Physics.h"
#include "Bang/Plane.h"
#include "Bang/Random.h"
#include "Bang/RectTransform.h"
#include "Bang/Assets.h"
#include "Bang/RigidBody.h"
#include "Bang/Scene.h"
#include "Bang/SphereCollider.h"
#include "Bang/UICanvas.h"
#include "Bang/UIFocusable.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UITextRenderer.h"
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
    GameObject *dirLightGo = GameObjectFactory::CreateGameObject();
    DirectionalLight *dirLight = dirLightGo->AddComponent<DirectionalLight>();
    dirLight->SetIntensity(5.0f);
    dirLight->SetCastShadows(false);
    dirLightGo->GetTransform()->SetPosition(Vector3(10, 10, 10));
    dirLightGo->GetTransform()->LookAt(Vector3::Zero());
    dirLightGo->SetParent(p_scene);

    p_focusable = AddComponent<UIFocusable>();
    p_focusable->SetConsiderForTabbing(false);

    GameObject *centralTextGo = GameObjectFactory::CreateUIGameObject();
    p_centralText = centralTextGo->AddComponent<UITextRenderer>();
    p_centralText->SetContent(
        "Please select a model or a mesh in the explorer");
    p_centralText->SetTextSize(21);
    p_centralText->SetTextColor(Color::White());

    GameObject *sceneCamGo = GameObjectFactory::CreateGameObject();
    p_sceneCamera = GameObjectFactory::CreateDefaultCameraInto(sceneCamGo);
    sceneCamGo->SetParent(p_scene);

    p_scene->SetCamera(p_sceneCamera);

    GameObject *lightGo = GameObjectFactory::CreateGameObject();
    lightGo->AddComponent<DirectionalLight>();
    lightGo->GetTransform()->LookInDirection(Vector3::Down());
    lightGo->SetParent(p_scene);

    GameObject *sceneImgGo = GameObjectFactory::CreateUIGameObject();
    p_sceneImg = sceneImgGo->AddComponent<UIImageRenderer>();
    p_sceneImg->SetMode(UIImageRenderer::Mode::TEXTURE);

    sceneImgGo->SetParent(this);
    centralTextGo->SetParent(this);

    ResetCamera();
}

EditorMeshFactoryUITabScene::~EditorMeshFactoryUITabScene()
{
}

void EditorMeshFactoryUITabScene::Update()
{
    GameObject::Update();

    bool justChangedModel = false;
    if (AH<Mesh> selectedMesh = GetExplorerSelectedMesh())
    {
        if (selectedMesh.Get() != GetCurrentMesh())
        {
            p_currentMesh.Set(selectedMesh.Get());
            p_currentModel.Set(nullptr);
            if (p_modelContainer)
            {
                GameObject::Destroy(p_modelContainer);
            }

            p_modelContainer = GameObjectFactory::CreateGameObject();
            MeshRenderer *mr = p_modelContainer->AddComponent<MeshRenderer>();
            mr->SetMesh(p_currentMesh.Get());
            justChangedModel = true;
        }
    }
    else if (AH<Model> selectedModel = GetExplorerSelectedModel())
    {
        if (selectedModel.Get() != GetCurrentModel())
        {
            p_currentMesh.Set(nullptr);
            p_currentModel.Set(selectedModel.Get());
            if (p_modelContainer)
            {
                GameObject::Destroy(p_modelContainer);
            }

            p_modelContainer = selectedModel.Get()->CreateGameObjectFromModel();
            justChangedModel = true;
        }
    }

    if (justChangedModel)
    {
        m_cameraOrbitPoint = p_modelContainer->GetBoundingSphere().GetCenter();
        p_modelContainer->SetParent(p_scene);
        ResetCamera();
    }

    p_centralText->GetGameObject()->SetEnabled(p_modelContainer == nullptr);

    Transform *camTR = p_sceneCamera->GetGameObject()->GetTransform();
    if (p_focusable->HasFocus())
    {
        if (!m_displacingModel && Input::GetMouseButton(MouseButton::LEFT))
        {
            Vector2 mouseCurrentAxisMovement = Input::GetMouseAxis();
            m_currentCameraRotAngles += mouseCurrentAxisMovement * 360.0f;

            m_currentCameraRotAngles.y =
                Math::Clamp(m_currentCameraRotAngles.y, -80.0f, 80.0f);
        }
        else if (Input::GetMouseButtonDown(MouseButton::RIGHT))
        {
            Vector3 orbitPoint = GetMousePointOverModel();
            if (orbitPoint != Vector3::Infinity())
            {
                m_cameraOrbitPoint = orbitPoint;
            }
        }

        m_currentCameraZoom +=
            (-Input::GetMouseWheel().y * 0.1f) * m_currentCameraZoom;
    }

    if (p_modelContainer && IsVisibleRecursively())
    {
        if (!p_modelContainer->GetComponentInDescendantsAndThis<MeshCollider>())
        {
            RigidBody *rb = p_modelContainer->AddComponent<RigidBody>();
            rb->SetIsKinematic(true);
            Array<MeshRenderer *> mrs = GetMeshRenderers();
            for (MeshRenderer *mr : mrs)
            {
                MeshCollider *meshCollider =
                    p_modelContainer->AddComponent<MeshCollider>();
                meshCollider->SetStatic(true);
                meshCollider->SetMesh(mr->GetActiveMesh());
            }
        }

        Sphere goSphere = p_modelContainer->GetBoundingSphere();
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
        Vector3 orbitPoint = m_cameraOrbitPoint;
        camTR->SetPosition(orbitPoint + camDir * camDist);
        camTR->LookAt(orbitPoint);

        p_sceneCamera->SetZNear(0.01f);
        p_sceneCamera->SetZFar((camDist + goSphere.GetRadius() * 2.0f) * 1.2f);

        m_lastModelDisplacementPoint = GetDisplacementPoint();
    }

    if (p_modelContainer)
    {
        if (Input::GetKeyDown(Key::C))
        {
            DoCollisionSimulation();
            GameObject *cube = GameObjectFactory::CreateCubeGameObject();

            RigidBody *rb = cube->AddComponent<RigidBody>();

            cube->SetParent(p_scene);

            AABox goAABox = p_modelContainer->GetAABBoxWorld();
            float radius = p_modelContainer->GetBoundingSphere().GetRadius();
            cube->GetTransform()->SetScale(radius * 0.2f);
            cube->GetTransform()->SetPosition(
                (radius * 0.5f + goAABox.GetSize().y) * Vector3::Up());
            // sphere->GetTransform()->Translate(Random::GetInsideUnitSphere() *
            //                                   goAABox.GetSize());
        }

        Physics *ph = Physics::GetInstance();
        {
            Time prevPhysicsStepTime = ph->GetStepSleepTime();
            uint prevPhysicsMaxSubSteps = ph->GetMaxSubSteps();
            {
                ph->SetMaxSubSteps(1);
                ph->SetStepSleepTime(Time::Seconds(0.5f));
                SceneManager::OnNewFrame(p_scene);
            }
            ph->SetMaxSubSteps(prevPhysicsMaxSubSteps);
            ph->SetStepSleepTime(prevPhysicsStepTime);
        }
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

    GEngine::GetInstance()->RenderToGBuffer(p_scene, p_sceneCamera);

    if (p_modelContainer)
    {
        p_sceneImg->SetTint(Color::White());
        p_sceneImg->SetImageTexture(
            p_sceneCamera->GetGBuffer()->GetDrawColorTexture());
    }
    else
    {
        p_sceneImg->SetTint(Color::DarkGray());
    }

    GL::Pop(GL::Pushable::VIEWPORT);
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
}

void EditorMeshFactoryUITabScene::ResetCamera()
{
    m_currentCameraZoom = 1.4f;
    m_currentCameraRotAngles = Vector2(45.0f, -45.0f);
    m_cameraOrbitPoint = Vector3::Zero();
}

Mesh *EditorMeshFactoryUITabScene::GetCurrentMesh() const
{
    return p_currentMesh.Get();
}

Model *EditorMeshFactoryUITabScene::GetCurrentModel() const
{
    return p_currentModel.Get();
}

void EditorMeshFactoryUITabScene::DoCollisionSimulation()
{
    if (!p_modelContainer)
    {
        return;
    }
}

Ray EditorMeshFactoryUITabScene::GetMouseRay() const
{
    const Vector2 mouseNDC = GetMouseNDC();
    const Ray camRay = p_sceneCamera->FromViewportPointNDCToRay(mouseNDC);
    return camRay;
}

Vector2 EditorMeshFactoryUITabScene::GetMouseNDC() const
{
    const Vector2 mouseNDC =
        p_sceneImg->GetGameObject()
            ->GetRectTransform()
            ->FromViewportPointToLocalPointNDC(Input::GetMousePosition());
    return mouseNDC;
}

Vector3 EditorMeshFactoryUITabScene::GetDisplacementPoint() const
{
    Ray ray = GetMouseRay();
    bool intersected;
    float distance;
    Geometry::IntersectRayPlane(
        ray, m_displacementPlane, &intersected, &distance);
    if (intersected)
    {
        Vector3 displacementPoint = ray.GetPoint(distance);
        return displacementPoint;
    }
    return Vector3::Zero();
}

Vector3 EditorMeshFactoryUITabScene::GetMousePointOverModel() const
{
    const Ray camRay = GetMouseRay();
    Vector3 minPoint = Vector3::Infinity();
    const Array<MeshRenderer *> mrs = GetMeshRenderers();
    for (MeshRenderer *mr : mrs)
    {
        bool intersectedLocal;
        Vector3 intersectionPoint;
        mr->IntersectRay(camRay, &intersectedLocal, &intersectionPoint);
        if (intersectedLocal)
        {
            if (Vector3::Distance(camRay.GetOrigin(), intersectionPoint) <
                Vector3::Distance(camRay.GetOrigin(), minPoint))
            {
                minPoint = intersectionPoint;
            }
        }
    }
    return minPoint;
}

Array<MeshRenderer *> EditorMeshFactoryUITabScene::GetMeshRenderers() const
{
    return p_modelContainer->GetComponentsInDescendantsAndThis<MeshRenderer>();
}

AH<Mesh> EditorMeshFactoryUITabScene::GetExplorerSelectedMesh() const
{
    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(Extensions::GetMeshExtension()))
        {
            AH<Mesh> mesh = Assets::Load<Mesh>(selectedPath);
            return mesh;
        }
    }
    return AH<Mesh>();
}

AH<Model> EditorMeshFactoryUITabScene::GetExplorerSelectedModel() const
{
    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(Extensions::GetModelExtensions()))
        {
            AH<Model> model = Assets::Load<Model>(selectedPath);
            return model;
        }
    }
    return AH<Model>();
}
