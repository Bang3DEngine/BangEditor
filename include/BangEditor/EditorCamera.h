#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"
#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

NAMESPACE_BANG_BEGIN
FORWARD class Input;
FORWARD class Camera;
NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class EditorCamera : public GameObject
{
    GAMEOBJECT_EDITOR(EditorCamera);

public:
    // GameObject
    void Start() override;
    void Update() override;

    void AlignViewWithGameObject(GameObject *selected);
    void SwitchProjectionModeTo(bool mode3D);
    void StartLookAt(GameObject *lookAtFocus);

    Camera *GetCamera();

private:
    static float InitialFOVDegrees;
    static float InitialZNear;
    static float InitialZFar;

    Camera *p_cam              = nullptr;
    Transform *p_camt          = nullptr;
    GameObject *p_camContainer = nullptr;

    // WASD
    float m_keysMoveAccel = 1.0f;
    float m_maxMoveSpeed  = 10.0f;
    float m_keysCurrentMoveSpeed = 0.0f;

    // Panning
    Vector2 m_mousePanPerPixel = Vector2(70.0f);

    // Rotation
    Quaternion m_startingRotation;                     // Starting rot offset
    Vector2 m_mouseRotDegreesAccum = Vector2(0.0f);    // User input
    Vector2 m_mouseRotDegreesPerPixel = Vector2(0.0f); // Parameter

    // Zoom
    float m_mouseZoomPerDeltaWheel = 0.2f;
    float m_zoomCurrentSpeed = 0.0f;

    float m_orthoHeight = 30.0f;

    // Focus
    GameObject *p_currentFocus = nullptr;
    float m_lookAtRotSpeed     = 3.0f;
    float m_lookAtMoveSpeed    = 4.0f;

    EditorCamera();
    virtual ~EditorCamera();

    void AdjustSpeeds();
    void UpdateRotationVariables();
    void HandleWheelZoom(Vector3 *moveStep, bool *hasMoved);
    bool HandleMouseRotation(bool *hasMoved, bool *unwrapMouse);
    void HandleMousePanning(bool *hasMoved, bool *unwrapMouse);
    void HandleKeyMovement(Vector3 *moveStep, bool *hasMoved);
    void HandleLookAtFocus();
};

NAMESPACE_BANG_EDITOR_END

#endif // EDITORCAMERA_H
