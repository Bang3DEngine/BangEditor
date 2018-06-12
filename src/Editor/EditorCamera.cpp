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
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/SelectionFramebuffer.h"
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
    p_cam->SetProjectionMode(Camera::ProjectionMode::PERSPECTIVE);
    p_cam->AddRenderPass(RenderPass::OVERLAY);

    p_camt = p_camContainer->GetTransform();
    p_cam->SetZNear(EditorCamera::InitialZNear);
    p_cam->SetZFar(EditorCamera::InitialZFar);
    p_cam->SetFovDegrees(EditorCamera::InitialFOVDegrees);

    m_selectionFramebuffer = new SelectionFramebuffer(1,1);
}

EditorCamera::~EditorCamera()
{
    delete m_selectionFramebuffer;
}

void EditorCamera::AdjustSpeeds()
{
    int wWidth  = Window::GetActive()->GetWidth();
    int wHeight = Window::GetActive()->GetHeight();

    m_mouseRotDegreesPerPixel.x = 360.0f / wWidth;
    m_mouseRotDegreesPerPixel.y = 360.0f / wHeight;

    m_mousePanPerPixel = Vector2(0.01f);
    if (p_currentFocus)
    {
        Transform *ft = p_currentFocus->GetTransform();
        if (ft)
        {
            Vector3 focusPoint = ft->GetPosition();
            float d = Vector3::Distance(focusPoint, GetTransform()->GetPosition());
            float ar = GL::GetViewportAspectRatio();
            float halfFov = Math::DegToRad(p_cam->GetFovDegrees()/2.0f);
            float halfHeightInWorldSpace = Math::Tan(halfFov) * d;
            m_mousePanPerPixel.y = (halfHeightInWorldSpace * 2) / wHeight;
            m_mousePanPerPixel.x = m_mousePanPerPixel.y * wHeight * ar / wWidth;
        }
    }

    m_mousePanPerPixel.x = Math::Max(m_mousePanPerPixel.x, 0.05f);
    m_mousePanPerPixel.y = Math::Max(m_mousePanPerPixel.y, 0.05f);
}

void EditorCamera::UpdateRotationVariables()
{
    m_mouseRotDegreesAccum = Vector2::Zero;
    m_startingRotation     = GetTransform()->GetLocalRotation();
}

void EditorCamera::HandleWheelZoom(Vector3 *moveStep, bool *hasMoved)
{
    // Update zoom value
    float mouseWheel = Input::GetMouseWheel().y * m_mouseZoomPerDeltaWheel;
    m_zoomCurrentSpeed = mouseWheel + (m_zoomCurrentSpeed * 0.5f); // Lerp

    // Apply zoom
    if (m_zoomCurrentSpeed != 0.0f)
    {
        if (p_cam->GetProjectionMode() == Camera::ProjectionMode::PERSPECTIVE)
        {
            *moveStep += m_zoomCurrentSpeed * p_camt->GetForward();
            *hasMoved  = (mouseWheel != 0.0f);
        }
        m_orthoHeight -= 2.75f * mouseWheel; // Magic number here :)
        p_cam->SetOrthoHeight(m_orthoHeight);
    }
}

bool EditorCamera::HandleMouseRotation(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(MouseButton::RIGHT))
    {
        Vector2 delta = Vector2(Input::GetMouseDelta()) * Vector2(-1, 1) *
                        m_mouseRotDegreesPerPixel;

        m_mouseRotDegreesAccum += delta;

        GetTransform()->SetLocalRotation(m_startingRotation);
        Quaternion rotX = Quaternion::AngleAxis(Math::DegToRad(m_mouseRotDegreesAccum.x),
                                                Vector3::Up);
        GetTransform()->Rotate(rotX);

        Quaternion rotY = Quaternion::AngleAxis(Math::DegToRad(m_mouseRotDegreesAccum.y),
                                                p_camt->GetRight());
        GetTransform()->Rotate(rotY);

        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
        return true;
    }
    return false;
}

void EditorCamera::HandleMousePanning(bool *hasMoved, bool *unwrapMouse)
{
    if (Input::GetMouseButton(MouseButton::MIDDLE))
    {
        Vector2 delta = -Vector2(Input::GetMouseDelta()) * m_mousePanPerPixel;

        GetTransform()->Translate(p_camt->GetRight() * delta.x + p_camt->GetUp() * delta.y);

        Input::SetMouseWrapping(true);
        *hasMoved  = true;
        *unwrapMouse = false;
    }
}

void EditorCamera::HandleKeyMovement(Vector3 *moveStep, bool *hasMoved)
{
    m_keysCurrentMoveSpeed += m_keysMoveAccel;
    m_keysCurrentMoveSpeed = Math::Min(m_keysCurrentMoveSpeed, m_maxMoveSpeed);

    if (Input::GetKey(Key::LCTRL) || Input::GetKey(Key::LSHIFT)) { return; }

    Vector3 m = Vector3::Zero;
    if (Input::GetKey(Key::W))
    {
        m += m_keysCurrentMoveSpeed * Time::GetDeltaTime() * p_camt->GetForward();
    }
    else if (Input::GetKey(Key::S))
    {
        m -= m_keysCurrentMoveSpeed * Time::GetDeltaTime() * p_camt->GetForward();
    }

    if (Input::GetKey(Key::A))
    {
        m -= m_keysCurrentMoveSpeed * Time::GetDeltaTime() * p_camt->GetRight();
    }
    else if (Input::GetKey(Key::D))
    {
        m += m_keysCurrentMoveSpeed * Time::GetDeltaTime() * p_camt->GetRight();
    }

    *moveStep += m;
    *hasMoved = *hasMoved || (m.Length() > 0);
}

void EditorCamera::HandleLookAtFocus()
{
    if (!p_currentFocus) { return; }

    Camera *cam = GetCamera();
    Sphere focusBSphere = p_currentFocus->GetBoundingSphere();

    Vector3 thisPos = GetTransform()->GetPosition();
    Vector3 focusPos = focusBSphere.GetCenter();
    Vector3 focusDir = (focusPos - thisPos).Normalized();

    //LookAt Rotation
    Quaternion originRot = GetTransform()->GetRotation();
    Quaternion destRot = Quaternion::LookDirection(focusDir, Vector3::Up);
    Quaternion rot = Quaternion::SLerp(originRot, destRot,
                                       Time::GetDeltaTime() * m_lookAtRotSpeed);
    GetTransform()->SetLocalRotation(rot);

    //LookAt Move
    float stopDist = 0.0f;
    float radius = focusBSphere.GetRadius();
    if (cam->GetProjectionMode() == Camera::ProjectionMode::PERSPECTIVE)
    {
        float fov = Math::DegToRad(cam->GetFovDegrees() / 2.0f);
        stopDist = radius / std::tan(fov) * 1.5f;
    }
    stopDist = std::max(stopDist, 1.0f); //In case boundingBox is empty
    Vector3 dest = focusPos - (focusDir * stopDist);
    float t = Time::GetDeltaTime() * m_lookAtMoveSpeed;
    GetTransform()->SetPosition( Vector3::Lerp(thisPos, dest, t) );

    if( Vector3::Distance(dest, thisPos) <= 0.1f &&
        Vector3::Dot(GetTransform()->GetForward(), focusDir) >= 0.99999f)
    {
        p_currentFocus = nullptr;
    }
}

void EditorCamera::OnStart()
{
    GameObject::OnStart();

    /*
    Sphere sceneSphere = SceneManager::GetActiveScene()->GetBoundingSphere();
    if (sceneSphere.GetRadius() > 0)
    {
        GetTransform()->SetPosition(Vector3(1.0f) * sceneSphere.GetRadius());
        GetTransform()->LookAt(sceneSphere.GetCenter());
    }
    else
    */
    {
        GetTransform()->SetPosition(Vector3::One * 8.0f);
        GetTransform()->LookAt(Vector3::Zero);
    }

    UpdateRotationVariables();
}

#include "Bang/GBuffer.h"
#include "Bang/PointLight.h"
void EditorCamera::Update()
{
    GameObject::Update();

    AdjustSpeeds();

    bool unwrapMouse = true;
    if (!IsBlocked())
    {
        if (!p_currentFocus)
        {
            bool hasMoved = false;
            Vector3 moveStep = Vector3::Zero;

            if ( GL::GetViewportRect().Contains( Input::GetMousePositionWindow() ))
            {
                HandleKeyMovement(&moveStep, &hasMoved); //WASD

                //Mouse rot with right click
                if (!HandleMouseRotation(&hasMoved, &unwrapMouse))
                {
                    //Mouse move with mid click
                    HandleMousePanning(&hasMoved, &unwrapMouse);
                }

                HandleWheelZoom(&moveStep, &hasMoved);

                if (!hasMoved)
                {
                    m_keysCurrentMoveSpeed = 0.0f; // Reset speed
                }

                GetTransform()->Translate(moveStep);
            }
        }
        else
        {
            UpdateRotationVariables();
            HandleLookAtFocus();
            UpdateRotationVariables();
        }
    }

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
        GetCamera()->SetClearMode(Camera::ClearMode::COLOR);
        GetCamera()->SetClearColor(Color::LightBlue);
    }

    if (unwrapMouse) { Input::SetMouseWrapping(false); }
}

void EditorCamera::AlignViewWithGameObject(GameObject *selected)
{
    p_currentFocus = nullptr;
    p_camt->SetLocalRotation(Quaternion::Identity);
    GetTransform()->SetPosition(selected->GetTransform()->GetPosition());
    Vector3 up = Vector3::Up;
    GetTransform()->LookInDirection(selected->GetTransform()->GetForward(), up);
    UpdateRotationVariables();
}

void EditorCamera::SwitchProjectionModeTo(bool mode3D)
{
    if (mode3D)
    {
        p_cam->SetProjectionMode(Camera::ProjectionMode::PERSPECTIVE);
        p_cam->SetZNear(EditorCamera::InitialZNear);
        p_cam->SetZFar(EditorCamera::InitialZFar);
        p_cam->SetFovDegrees(EditorCamera::InitialFOVDegrees);
    }
    else
    {
        p_cam->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
        p_cam->SetOrthoHeight(m_orthoHeight);
        p_cam->SetZNear(-999999.9f);
        p_cam->SetZFar(999999.9f);
    }
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
