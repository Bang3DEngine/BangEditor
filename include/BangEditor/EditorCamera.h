#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"
#include "Bang/GameObject.h"
#include "Bang/IEventsSceneManager.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Input;
FORWARD class Camera;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class SelectionFramebuffer;

class EditorCamera : public GameObject,
                     public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(EditorCamera);

public:
    EditorCamera();
    virtual ~EditorCamera();

    // GameObject
    void Update() override;

    void FocusScene(Scene *scene);
    void AlignViewWithGameObject(GameObject *selected);
    void SwitchProjectionModeTo(bool mode3D);
    void LookAt(GameObject *lookAtFocus);
    void RequestBlockBy(GameObject *go);
    void RequestUnBlockBy(GameObject *go);
    void SetZoomSpeedMultiplier(float zoomSpeedMultiplier);

    SelectionFramebuffer *GetSelectionFramebuffer() const;
    Camera *GetCamera() const;
    bool IsBlocked() const;

    static EditorCamera *GetInstance();

private:
    static float InitialFOVDegrees;
    static float InitialZNear;
    static float InitialZFar;

    Camera *p_cam              = nullptr;
    Transform *p_camt          = nullptr;
    GameObject *p_camContainer = nullptr;
    SelectionFramebuffer *m_selectionFramebuffer = nullptr;
    Set<GameObject*> m_blockRequests;

    Vector3 m_targetPosition = Vector3::Zero;
    Quaternion m_targetRotation = Quaternion::Identity;

    // WASD
    float m_keysMoveAccel = 1.0f;
    float m_maxMoveSpeed  = 10.0f;
    float m_keysCurrentMoveSpeed = 0.0f;

    // Panning
    Vector2 m_mousePanPerPixel = Vector2(70.0f);

    // Rotation
    Vector2 m_mouseRotDegreesPerPixel = Vector2(0.0f); // Parameter

    // Zoom
    float m_mouseZoomPerDeltaWheel = 0.0f;
    float m_zoomSpeedMultiplier = 0.0f;

    float m_orthoHeight = 30.0f;

    // Focus
    GameObject *p_currentLookAtGo = nullptr;
    float m_lookAtRotSpeed     = 3.0f;
    float m_lookAtMoveSpeed    = 4.0f;

    // IEventsSceneManager
    virtual void OnSceneLoaded(Scene *scene, const Path &sceneFilepath) override;

    void AdjustSpeeds();
    void HandleWheelZoom();
    bool HandleMouseRotation();
    void HandleMousePanning();
    void HandleKeyMovement();
    void HandleLookAtFocus();
    void InterpolatePositionAndRotation(double extraInterpolationPos = 0.0,
                                        double extraInterpolationRot = 0.0);
    void GetLookAtFocusParams(GameObject *lookAtGo,
                              Vector3 *targetPos,
                              Quaternion *targetRot);
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORCAMERA_H
