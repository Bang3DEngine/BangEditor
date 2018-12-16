#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H

#include <functional>
#include <vector>

#include "Bang/Array.tcc"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/GameObject.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsSceneManager.h"
#include "Bang/Quaternion.h"
#include "Bang/Set.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/IEventsScenePlayer.h"
#include "BangEditor/PlayState.h"
#include "BangEditor/Selection.h"

namespace Bang
{
class Camera;
class IEventsSceneManager;
class Input;
class Path;
class Scene;
class Transform;
}

using namespace Bang;
namespace BangEditor
{
class IEventsScenePlayer;

class EditorCamera : public GameObject,
                     public EventListener<IEventsScenePlayer>,
                     public EventListener<IEventsSceneManager>
{
    GAMEOBJECT_EDITOR(EditorCamera);

public:
    EditorCamera();
    virtual ~EditorCamera() override;

    // GameObject
    void Render(RenderPass rp, bool renderChildren) override;
    void Update() override;

    void FocusScene(Scene *scene);
    void SetSeeActiveCameraPostProcessEffects(bool seePostProcess);
    void SetPositionDirectly(const Vector3 &position);
    void SetRotationDirectly(const Quaternion &rotation);
    void AlignViewWithGameObject(GameObject *selected);
    void SwitchProjectionModeTo(bool mode3D);
    void LookAt(GameObject *lookAtFocus);
    void RequestBlockBy(GameObject *go);
    void RequestUnBlockBy(GameObject *go);
    void SetZoomSpeedMultiplier(float zoomSpeedMultiplier);

    Selection *GetSelection() const;
    Camera *GetCamera() const;
    bool IsBlocked() const;

    static EditorCamera *GetInstance();

private:
    static float InitialFOVDegrees;
    static float InitialZNear;
    static float InitialZFar;

    Camera *p_cam = nullptr;
    Transform *p_camt = nullptr;
    Selection *p_selection = nullptr;
    GameObject *p_camContainer = nullptr;
    Set<GameObject *> m_blockRequests;

    bool m_seeActiveCameraPostProcessEffects = true;

    Vector3 m_targetPosition = Vector3::Zero();
    Quaternion m_targetRotation = Quaternion::Identity();

    Vector3 m_previousPlayStateChangePos = Vector3::Zero();
    Quaternion m_previousPlayStateChangeRot = Quaternion::Identity();

    // WASD
    float m_keysMoveAccel = 1.0f;
    float m_maxMoveSpeed = 10.0f;
    float m_keysCurrentMoveSpeed = 0.0f;

    // Panning
    Vector2 m_mousePanPerPixel = Vector2(70.0f);

    // Rotation
    Vector2 m_mouseRotDegreesPerPixel = Vector2(0.0f);  // Parameter

    // Zoom
    float m_mouseZoomPerDeltaWheel = 0.0f;
    float m_zoomSpeedMultiplier = 0.0f;

    float m_orthoHeight = 30.0f;

    // Focus
    float m_lookAtRotSpeed = 3.0f;
    float m_lookAtMoveSpeed = 4.0f;

    // IEventsScenePlayer
    virtual void OnPlayStateChanged(PlayState previousPlayState,
                                    PlayState newPlayState) override;

    // IEventsSceneManager
    virtual void OnSceneLoaded(Scene *scene,
                               const Path &sceneFilepath) override;

    void AdjustSpeeds();
    void HandleWheelZoom();
    bool HandleMouseRotation();
    void HandleMousePanning();
    void HandleKeyMovement();
    void InterpolatePositionAndRotation(double extraInterpolationPos = 0.0,
                                        double extraInterpolationRot = 0.0);
    void GetLookAtFocusParams(GameObject *lookAtGo,
                              Vector3 *targetPos,
                              Quaternion *targetRot);
};
}

#endif  // EDITORCAMERA_H
