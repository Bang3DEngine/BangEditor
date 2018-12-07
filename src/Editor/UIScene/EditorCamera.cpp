#include "BangEditor/EditorCamera.h"

#include <cmath>

#include "Bang/Camera.h"
#include "Bang/Color.h"
#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GL.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/HideFlags.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Input.h"
#include "Bang/Key.h"
#include "Bang/Math.h"
#include "Bang/MouseButton.h"
#include "Bang/Quaternion.h"
#include "Bang/RenderPass.h"
#include "Bang/Scene.h"
#include "Bang/SceneManager.h"
#include "Bang/Set.tcc"
#include "Bang/Sphere.h"
#include "Bang/Time.h"
#include "Bang/Transform.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Window.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/ScenePlayer.h"
#include "BangEditor/UISceneEditContainer.h"

namespace Bang
{
class Path;
}

using namespace Bang;
using namespace BangEditor;

float EditorCamera::InitialFOVDegrees = 60.0f;
float EditorCamera::InitialZNear = 0.5f;
float EditorCamera::InitialZFar = 9999.9f;

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

    p_selection = new Selection();

    SceneManager::GetActive()
        ->EventEmitter<IEventsSceneManager>::RegisterListener(this);
    ScenePlayer::GetInstance()
        ->EventEmitter<IEventsScenePlayer>::RegisterListener(this);
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::Update()
{
    GameObject::Update();

    p_selection->OnNewFrame();
    AdjustSpeeds();

    bool unwrapMouse = true;
    if (!IsBlocked())
    {
        if (GL::GetViewportRect().Contains(Input::GetMousePositionWindow()))
        {
            HandleKeyMovement();  // WASD

            // Mouse rot with right click
            if (!HandleMouseRotation())
            {
                // Mouse move with mid click
                HandleMousePanning();
            }

            HandleWheelZoom();

            m_keysCurrentMoveSpeed = 0.0f;  // Reset speed
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
        // GetCamera()->SetSkyBoxTexture(sceneCam->GetSpecularSkyBoxTexture(),
        // false);
        // GetCamera()->SetSkyBoxTexture(sceneCam->GetDiffuseSkyBoxTexture(),
        // false);
    }
    else
    {
        GetCamera()->SetClearMode(CameraClearMode::COLOR);
        GetCamera()->SetClearColor(Color::White().WithValue(0.3f));
    }

    if (unwrapMouse)
    {
        Input::SetMouseWrapping(false);
    }
}

void EditorCamera::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);
}

void EditorCamera::AdjustSpeeds()
{
    int wWidth = Window::GetActive()->GetWidth();
    int wHeight = Window::GetActive()->GetHeight();

    m_mouseRotDegreesPerPixel.x = 360.0f / wWidth;
    m_mouseRotDegreesPerPixel.y = 360.0f / wHeight;

    m_mousePanPerPixel = Vector2(0.01f);

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
    float posT =
        SCAST<float>(Math::Min((dts * interpPosSpeed) + extraInterpPos, 1.0));
    Vector3 newPos =
        Vector3::Lerp(GetTransform()->GetPosition(), m_targetPosition, posT);
    GetTransform()->SetLocalPosition(newPos);

    float interpRotSpeed = 3.0f;
    float rotT =
        SCAST<float>(Math::Min((dts * interpRotSpeed) + extraInterpRot, 1.0));
    Quaternion newRot = Quaternion::SLerp(
        GetTransform()->GetRotation(), m_targetRotation, rotT);
    GetTransform()->SetLocalRotation(newRot);
}

void EditorCamera::HandleWheelZoom()
{
    // Update zoom value
    if (UISceneEditContainer::IsMouseOver())
    {
        float mouseWheel = Input::GetMouseWheel().y * m_mouseZoomPerDeltaWheel;
        float zoomSpeed = mouseWheel * m_zoomSpeedMultiplier;

        // Apply zoom
        if (zoomSpeed != 0.0f)
        {
            if (p_cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
            {
                SetPositionDirectly(GetTransform()->GetPosition() +
                                    (zoomSpeed * p_camt->GetForward()));
            }
            m_orthoHeight -= 2.75f * mouseWheel;  // Magic number here :)
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
        Quaternion rotX =
            Quaternion::AngleAxis(Math::DegToRad(delta.x), Vector3::Up());
        newRot = rotX * newRot;
        Quaternion rotY = Quaternion::AngleAxis(Math::DegToRad(delta.y),
                                                newRot * Vector3::Right());
        newRot = rotY * newRot;

        SetRotationDirectly(newRot);

        Input::SetMouseWrapping(true);

        return true;
    }
    return false;
}

void EditorCamera::HandleMousePanning()
{
    if (Input::GetMouseButton(MouseButton::MIDDLE))
    {
        Vector2 delta = -Vector2(Input::GetMouseDelta()) * m_mousePanPerPixel *
                        m_zoomSpeedMultiplier;
        SetPositionDirectly(
            GetTransform()->GetPosition() +
            (p_camt->GetRight() * delta.x + p_camt->GetUp() * delta.y));
        Input::SetMouseWrapping(true);
    }
}

void EditorCamera::HandleKeyMovement()
{
    m_keysCurrentMoveSpeed += m_keysMoveAccel;
    m_keysCurrentMoveSpeed = Math::Min(m_keysCurrentMoveSpeed, m_maxMoveSpeed);

    if (!Input::GetKey(Key::LCTRL) && !Input::GetKey(Key::LSHIFT))
    {
        float dt = SCAST<float>(Time::GetDeltaTime().GetSeconds());
        Vector3 m = Vector3::Zero();
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
        Vector3 sceneCenter = Vector3::Zero();

        Sphere bSphere = scene->GetBoundingSphere();
        if (!Math::IsInfinity(bSphere.GetRadius()) &&
            !Math::IsNaN(bSphere.GetRadius()))
        {
            sceneRadius = bSphere.GetRadius();
            sceneCenter = bSphere.GetCenter();
        }

        m_targetPosition = (sceneCenter + Vector3::One() * sceneRadius * 1.1f);
        m_targetRotation =
            Quaternion::LookDirection(sceneCenter - m_targetPosition);
    }
}

void EditorCamera::SetPositionDirectly(const Vector3 &position)
{
    m_targetPosition = position;
    InterpolatePositionAndRotation(1, 0);
}

void EditorCamera::SetRotationDirectly(const Quaternion &rotation)
{
    m_targetRotation = rotation;
    InterpolatePositionAndRotation(0, 1);
}

void EditorCamera::AlignViewWithGameObject(GameObject *selected)
{
    p_camt->SetLocalRotation(Quaternion::Identity());
    m_targetPosition = selected->GetTransform()->GetPosition();
    Vector3 up = Vector3::Up();
    m_targetRotation =
        Quaternion::LookDirection(selected->GetTransform()->GetForward(), up);
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
    if (lookAtFocus)
    {
        Vector3 targetPos;
        Quaternion targetRot;
        GetLookAtFocusParams(lookAtFocus, &targetPos, &targetRot);

        m_targetPosition = targetPos;
        m_targetRotation = targetRot;
    }
}

void EditorCamera::GetLookAtFocusParams(GameObject *lookAtGo,
                                        Vector3 *targetPos,
                                        Quaternion *targetRot)
{
    Camera *cam = GetCamera();
    Sphere focusBSphere = lookAtGo->GetBoundingSphere();

    Vector3 thisPos = GetTransform()->GetPosition();
    Vector3 focusPos = (!Math::IsInfinity(focusBSphere.GetRadius())
                            ? focusBSphere.GetCenter()
                            : lookAtGo->GetTransform()->GetPosition());
    Vector3 focusDir = (focusPos - thisPos).NormalizedSafe();

    // LookAt Rotation
    *targetRot = Quaternion::LookDirection(focusDir, Vector3::Up());

    // LookAt Move
    float radius = focusBSphere.GetRadius();
    radius = !Math::IsInfinity(radius) ? radius : 0.1f;
    radius = Math::Max(radius, 0.1f);
    float stopDist = radius;
    if (cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
    {
        float fov = Math::DegToRad(cam->GetFovDegrees() / 2.0f);
        stopDist = radius / std::tan(fov) * 1.5f;
    }
    stopDist = Math::Max(stopDist, 1.0f);
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

void EditorCamera::SetZoomSpeedMultiplier(float zoomSpeedMultiplier)
{
    m_zoomSpeedMultiplier = zoomSpeedMultiplier;
}

Selection *EditorCamera::GetSelection() const
{
    return p_selection;
}

bool EditorCamera::IsBlocked() const
{
    return !m_blockRequests.IsEmpty();
}
Camera *EditorCamera::GetCamera() const
{
    return p_camContainer->GetComponent<Camera>();
}

EditorCamera *EditorCamera::GetInstance()
{
    EditSceneGameObjects *editSceneGameObject =
        EditSceneGameObjects::GetInstance();
    return editSceneGameObject ? editSceneGameObject->GetEditorCamera()
                               : nullptr;
}

void EditorCamera::OnPlayStateChanged(PlayState prevPlayState,
                                      PlayState newPlayState)
{
    switch (newPlayState)
    {
        case PlayState::JUST_BEFORE_PLAYING:
            m_previousPlayStateChangePos = GetTransform()->GetPosition();
            m_previousPlayStateChangeRot = GetTransform()->GetRotation();
            break;

        case PlayState::PLAYING:
            if (prevPlayState == PlayState::JUST_BEFORE_PLAYING)
            {
                SetPositionDirectly(m_previousPlayStateChangePos);
                SetRotationDirectly(m_previousPlayStateChangeRot);
            }
            break;

        case PlayState::EDITING:
            SetPositionDirectly(m_previousPlayStateChangePos);
            SetRotationDirectly(m_previousPlayStateChangeRot);
            break;

        default: break;
    }
}

void EditorCamera::OnSceneLoaded(Scene *scene, const Path &sceneFilepath)
{
    BANG_UNUSED(sceneFilepath);
    EditorCamera::GetInstance()->FocusScene(scene);
}
