#include "BangEditor/EditorMeshFactoryUITabScene.h"

#include "Bang/Camera.h"
#include "Bang/DirectionalLight.h"
#include "Bang/Extensions.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/Geometry.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Model.h"
#include "Bang/Plane.h"
#include "Bang/RectTransform.h"
#include "Bang/Resources.h"
#include "Bang/Scene.h"
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
    dirLight->SetShadowType(Light::ShadowType::NONE);
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

    if (RH<Mesh> selectedMesh = GetExplorerSelectedMesh())
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
        }
    }
    else if (RH<Model> selectedModel = GetExplorerSelectedModel())
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
            ResetCamera();
        }
    }

    p_centralText->GetGameObject()->SetEnabled(p_modelContainer == nullptr);

    Transform *camTR = p_sceneCamera->GetGameObject()->GetTransform();
    if (p_focusable->HasFocus())
    {
        if (!m_displacingModel && Input::GetMouseButton(MouseButton::LEFT))
        {
            Vector2 mouseCurrentAxisMovement = Input::GetMouseAxis();
            m_currentCameraRotAngles += mouseCurrentAxisMovement * 360.0f;
        }
        else if (Input::GetMouseButtonDown(MouseButton::RIGHT))
        {
            Vector3 displacementPoint = GetMousePointOverModel();
            if (displacementPoint == Vector3::Infinity())
            {
                displacementPoint = Vector3::Zero();
            }

            m_displacementPlane = Plane(displacementPoint, camTR->GetBack());
            m_displacingModel = true;
            m_lastModelDisplacementPoint = GetDisplacementPoint();
        }

        if (Input::GetMouseButtonUp(MouseButton::RIGHT))
        {
            m_displacingModel = false;
        }

        if (m_displacingModel)
        {
            Vector3 currentDisplacementPoint = GetDisplacementPoint();
            Vector3 displacement =
                (currentDisplacementPoint - m_lastModelDisplacementPoint);
            Debug_DPeek(currentDisplacementPoint);
            Debug_DPeek(m_lastModelDisplacementPoint);
            Debug_DPeek(displacement);
            Debug_DLog("---------------");
            m_cameraOrbitPointOffset -= displacement;
        }
    }

    m_currentCameraZoom +=
        (-Input::GetMouseWheel().y * 0.1f) * m_currentCameraZoom;

    m_currentCameraRotAngles.y =
        Math::Clamp(m_currentCameraRotAngles.y, -80.0f, 80.0f);

    if (p_modelContainer)
    {
        p_modelContainer->SetParent(p_scene);

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
        Vector3 orbitPoint = goSphere.GetCenter() + m_cameraOrbitPointOffset;
        camTR->SetPosition(orbitPoint + camDir * camDist);
        camTR->LookAt(orbitPoint);

        p_sceneCamera->SetZNear(0.01f);
        p_sceneCamera->SetZFar((camDist + goSphere.GetRadius() * 2.0f) * 1.2f);

        m_lastModelDisplacementPoint = GetDisplacementPoint();
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
            p_sceneCamera->GetGBuffer()->GetLastDrawnColorTexture());
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
    m_cameraOrbitPointOffset = Vector3::Zero();
}

Mesh *EditorMeshFactoryUITabScene::GetCurrentMesh() const
{
    return p_currentMesh.Get();
}

Model *EditorMeshFactoryUITabScene::GetCurrentModel() const
{
    return p_currentModel.Get();
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
    }
    return RH<Mesh>();
}

RH<Model> EditorMeshFactoryUITabScene::GetExplorerSelectedModel() const
{
    if (Explorer *exp = Explorer::GetInstance())
    {
        Path selectedPath = exp->GetSelectedPath();
        if (selectedPath.HasExtension(Extensions::GetModelExtensions()))
        {
            RH<Model> model = Resources::Load<Model>(selectedPath);
            return model;
        }
    }
    return RH<Model>();
}
