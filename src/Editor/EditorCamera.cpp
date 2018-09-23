#include "BangEditor/EditorCamera.h"

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Sphere.h"
#include "Bang/Window.h"
#include "Bang/UICanvas.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/SelectionFramebuffer.h"
#include "BangEditor/UISceneEditContainer.h"
#include "BangEditor/NotSelectableInEditor.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

float EditorCamera::InitialFOVDegrees = 60.0f;
float EditorCamera::InitialZNear      = 0.5f;
float EditorCamera::InitialZFar       = 9999.9f;

EditorCamera::EditorCamera()
{
    SetName("EditorCamera");
    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DONT_SERIALIZE);
    GetHideFlags().SetOn(HideFlag::DONT_CLONE);

    p_camContainer = GameObjectFactory::CreateGameObject();
    p_camContainer->SetName("CameraContainer");
    p_camContainer->SetParent(this);

    p_cam = p_camContainer->AddComponent<Camera>();
    p_cam->SetProjectionMode(CameraProjectionMode::PERSPECTIVE);
    p_cam->AddRenderPass(RenderPass::OVERLAY);

    p_camt = p_camContainer->GetTransform();
    p_cam->SetZNear(EditorCamera::InitialZNear);
    p_cam->SetZFar(EditorCamera::InitialZFar);
    p_cam->SetFovDegrees(EditorCamera::InitialFOVDegrees);

    m_selectionFramebuffer = new SelectionFramebuffer(1,1);

    SceneManager::GetActive()->RegisterListener(this);
}

EditorCamera::~EditorCamera()
{
    delete m_selectionFramebuffer;
}

void EditorCamera::Update()
{
    GameObject::Update();

    AdjustSpeeds();

    bool unwrapMouse = true;
    if (!IsBlocked())
    {
        if (!p_currentLookAtGo)
        {
            if ( GL::GetViewportRect().Contains( Input::GetMousePositionWindow() ))
            {
                HandleKeyMovement(); //WASD

                //Mouse rot with right click
                if (!HandleMouseRotation())
                {
                    //Mouse move with mid click
                    HandleMousePanning();
                }

                HandleWheelZoom();

                m_keysCurrentMoveSpeed = 0.0f; // Reset speed
            }
        }
        else
        {
            HandleLookAtFocus();
        }
    }
    InterpolatePositionAndRotation();

    // Copy clear mode stuff from current scene camera
    Scene *scene = EditorSceneManager::GetOpenScene();
    Camera *sceneCam = scene ? scene->GetCamera() : nullptr;
    if (sceneCam)
    {
        GetCamera()->SetClearMode(sceneCam->GetClearMode());
        GetCamera()->SetClearColor(sceneCam->GetClearColor());
        GetCamera()->SetSkyBoxTexture(sceneCam->GetSkyBoxTexture());
        // GetCamera()->SetSkyBoxTexture(sceneCam->GetSpecularSkyBoxTexture(), false);
        // GetCamera()->SetSkyBoxTexture(sceneCam->GetDiffuseSkyBoxTexture(), false);
    }
    else
    {
        GetCamera()->SetClearMode(CameraClearMode::COLOR);
        GetCamera()->SetClearColor(Color::White.WithValue(0.3f));
    }

    if (unwrapMouse)
    {
        Input::SetMouseWrapping(false);
    }
}

void EditorCamera::AdjustSpeeds()
{
    int wWidth  = Window::GetActive()->GetWidth();
    int wHeight = Window::GetActive()->GetHeight();

    m_mouseRotDegreesPerPixel.x = 360.0f / wWidth;
    m_mouseRotDegreesPerPixel.y = 360.0f / wHeight;

    m_mousePanPerPixel = Vector2(0.01f);
    if (p_currentLookAtGo)
    {
        Transform *ft = p_currentLookAtGo->GetTransform();
        if (ft)
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, m_targetPosition);
            float ar = GL::GetViewportAspectRatio();
            float halfFov = Math::DegToRad(p_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = Math::Tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / wHeight;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * wHeight * ar / wWidth;
            m_mouseZoomPerDeltaWheel = Math::Clamp(d, 0.1f, 5.0f);
        }
    }

    m_mouseZoomPerDeltaWheel = 1.0f;
    m_mousePanPerPixel.x = Math::Max(m_mousePanPerPixel.x, 0.05f);
    m_mousePanPerPixel.y = Math::Max(m_mousePanPerPixel.y, 0.05f);
}

void EditorCamera::InterpolatePositionAndRotation(double extraInterpPos,
                                                  double extraInterpRot)
{
    Time dt = Time::GetDeltaTime();
    double dts = dt.GetSeconds();

    double interpPosSpeed = 2.0;
    float posT = Math::Min((dts * interpPosSpeed) + extraInterpPos, 1.0);
    Vector3 newPos =
        Vector3::Lerp(GetTransform()->GetPosition(), m_targetPosition, posT);
    GetTransform()->SetLocalPosition(newPos);

    float interpRotSpeed = 3.0f;
    float rotT = Math::Min((dts * interpRotSpeed) + extraInterpRot, 1.0);
    Quaternion newRot =
       Quaternion::SLerp(GetTransform()->GetRotation(), m_targetRotation, rotT);
    GetTransform()->SetLocalRotation(newRot);
}

void EditorCamera::HandleWheelZoom()
{
    // Update zoom value
    if (UISceneEditContainer::IsMouseOver())
    {
        float mouseWheel = Input::GetMouseWheel().y * m_mouseZoomPerDeltaWheel;
        m_zoomCurrentSpeed = mouseWheel + (m_zoomCurrentSpeed * 0.5f); // Lerp

        // Apply zoom
        if (m_zoomCurrentSpeed != 0.0f)
        {
            if (p_cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
            {
                m_targetPosition += m_zoomCurrentSpeed * p_camt->GetForward();
            }
            m_orthoHeight -= 2.75f * mouseWheel; // Magic number here :)
            p_cam->SetOrthoHeight(m_orthoHeight);
        }
    }
}

bool EditorCamera::HandleMouseRotation()
{
    if (Input::GetMouseButton(MouseButton::RIGHT) &&
        UISceneEditContainer::IsMouseOver())
    {
        Vector2 delta = Vector2(Input::GetMouseDelta()) * Vector2(-1, 1) *
                        m_mouseRotDegreesPerPixel;

        Quaternion newRot = GetTransform()->GetRotation();
        Quaternion rotX = Quaternion::AngleAxis(Math::DegToRad(delta.x),
                                                Vector3::Up);
        newRot = rotX * newRot;
        Quaternion rotY = Quaternion::AngleAxis(Math::DegToRad(delta.y),
                                                newRot * Vector3::Right);
        newRot = rotY * newRot;

        m_targetRotation = newRot;

        Input::SetMouseWrapping(true);
        InterpolatePositionAndRotation(0.0f, 1.0f);

        return true;
    }
    return false;
}

void EditorCamera::HandleMousePanning()
{
    if (Input::GetMouseButton(MouseButton::MIDDLE))
    {
        Vector2 delta = -Vector2(Input::GetMouseDelta()) * m_mousePanPerPixel;
        m_targetPosition = GetTransform()->GetPosition() +
                           (p_camt->GetRight() * delta.x + p_camt->GetUp() * delta.y);
        InterpolatePositionAndRotation(1.0f);
        Input::SetMouseWrapping(true);
    }
}

void EditorCamera::HandleKeyMovement()
{
    m_keysCurrentMoveSpeed += m_keysMoveAccel;
    m_keysCurrentMoveSpeed = Math::Min(m_keysCurrentMoveSpeed, m_maxMoveSpeed);

    if (!Input::GetKey(Key::LCTRL) && !Input::GetKey(Key::LSHIFT))
    {
        float dt = Time::GetDeltaTime().GetSeconds();
        Vector3 m = Vector3::Zero;
        if (Input::GetKey(Key::W))
        {
            m += m_keysCurrentMoveSpeed * dt * p_camt->GetForward();
        }
        else if (Input::GetKey(Key::S))
        {
            m -= m_keysCurrentMoveSpeed * dt * p_camt->GetForward();
        }

        if (Input::GetKey(Key::A))
        {
            m -= m_keysCurrentMoveSpeed * dt * p_camt->GetRight();
        }
        else if (Input::GetKey(Key::D))
        {
            m += m_keysCurrentMoveSpeed * dt * p_camt->GetRight();
        }

        m_targetPosition += m;
    }
}

void EditorCamera::FocusScene(Scene *scene)
{
    if (scene)
    {
        float sceneRadius = 1.0f;
        Vector3 sceneCenter = Vector3::Zero;

        Sphere bSphere = scene->GetBoundingSphere();
        if ( !Math::IsInfinity(bSphere.GetRadius()) )
        {
            sceneRadius = bSphere.GetRadius();
            sceneCenter = bSphere.GetCenter();
        }

        m_targetPosition = (sceneCenter + Vector3::One * sceneRadius * 1.1f);
        m_targetRotation = Quaternion::LookDirection(sceneCenter - m_targetPosition);
    }
}

void EditorCamera::AlignViewWithGameObject(GameObject *selected)
{
    p_currentLookAtGo = nullptr;
    p_camt->SetLocalRotation(Quaternion::Identity);
    m_targetPosition = selected->GetTransform()->GetPosition();
    Vector3 up = Vector3::Up;
    m_targetRotation = Quaternion::LookDirection(selected->GetTransform()->GetForward(), up);
}

void EditorCamera::SwitchProjectionModeTo(bool mode3D)
{
    if (mode3D)
    {
        p_cam->SetProjectionMode(CameraProjectionMode::PERSPECTIVE);
        p_cam->SetZNear(EditorCamera::InitialZNear);
        p_cam->SetZFar(EditorCamera::InitialZFar);
        p_cam->SetFovDegrees(EditorCamera::InitialFOVDegrees);
    }
    else
    {
        p_cam->SetProjectionMode(CameraProjectionMode::ORTHOGRAPHIC);
        p_cam->SetOrthoHeight(m_orthoHeight);
        p_cam->SetZNear(-999999.9f);
        p_cam->SetZFar(999999.9f);
    }
}

void EditorCamera::LookAt(GameObject *lookAtFocus)
{
    p_currentLookAtGo = lookAtFocus;

    if (p_currentLookAtGo)
    {
        Vector3 targetPos;
        Quaternion targetRot;
        GetLookAtFocusParams(p_currentLookAtGo, &targetPos, &targetRot);

        m_targetPosition = targetPos;
        m_targetRotation = targetRot;
    }
}

void EditorCamera::HandleLookAtFocus()
{
    if (p_currentLookAtGo)
    {
        Vector3 targetPos;
        Quaternion targetRot;
        GetLookAtFocusParams(p_currentLookAtGo, &targetPos, &targetRot);

        Vector3 thisPos = GetTransform()->GetPosition();
        Vector3 targetDir = targetRot * Vector3::Forward;
        if( Vector3::Distance(thisPos, targetPos) <= 0.1f &&
            Vector3::Dot(GetTransform()->GetForward(), targetDir) >= 0.9999f)
        {
            p_currentLookAtGo = nullptr;
        }
    }
}

void EditorCamera::GetLookAtFocusParams(GameObject *lookAtGo,
                                        Vector3 *targetPos,
                                        Quaternion *targetRot)
{
    Camera *cam = GetCamera();
    Sphere focusBSphere = lookAtGo->GetBoundingSphere();

    Vector3 thisPos = GetTransform()->GetPosition();
    Vector3 focusPos = (!Math::IsInfinity(focusBSphere.GetRadius()) ?
            focusBSphere.GetCenter() : lookAtGo->GetTransform()->GetPosition());
    Vector3 focusDir = (focusPos - thisPos).Normalized();

    // LookAt Rotation
    *targetRot = Quaternion::LookDirection(focusDir, Vector3::Up);

    // LookAt Move
    float radius = !Math::IsInfinity(radius) ? focusBSphere.GetRadius() : 0.1f;
    radius = Math::Max(radius, 0.1f);
    float stopDist = radius;
    if (cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
    {
        float fov = Math::DegToRad(cam->GetFovDegrees() / 2.0f);
        stopDist = radius / std::tan(fov) * 1.5f;
    }
    stopDist = Math::Max(stopDist, 0.5f);
    *targetPos = focusPos - (focusDir * stopDist);
}

void EditorCamera::RequestBlockBy(GameObject *go)
{
    m_blockRequests.Add(go);
}

void EditorCamera::RequestUnBlockBy(GameObject *go)
{
    m_blockRequests.Remove(go);
}

bool EditorCamera::IsBlocked() const
{
    return !m_blockRequests.IsEmpty();
}

SelectionFramebuffer* EditorCamera::GetSelectionFramebuffer() const
{
    return m_selectionFramebuffer;
}

Camera *EditorCamera::GetCamera() const
{
    return p_camContainer->GetComponent<Camera>();
}

EditorCamera *EditorCamera::GetInstance()
{
    EditSceneGameObjects *editSceneGameObject = EditSceneGameObjects::GetInstance();
    return editSceneGameObject ? editSceneGameObject->GetEditorCamera() : nullptr;
}

void EditorCamera::OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
{
    BANG_UNUSED(sceneFilepath);
    EditorCamera::GetInstance()->FocusScene(scene);
}
