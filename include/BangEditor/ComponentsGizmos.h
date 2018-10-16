#ifndef COMPONENTSGIZMOS_H
#define COMPONENTSGIZMOS_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang {
class Camera;
class Component;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class AudioSource;
FORWARD class BoxCollider;
FORWARD class CapsuleCollider;
FORWARD class DirectionalLight;
FORWARD class ParticleSystem;
FORWARD class PointLight;
FORWARD class ReflectionProbe;
FORWARD class Rope;
FORWARD class SphereCollider;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ComponentsGizmos : public GameObject
{
    GAMEOBJECT_EDITOR(ComponentsGizmos);

public:
    ComponentsGizmos();
    virtual ~ComponentsGizmos();

    // GameObject
    void Render(RenderPass rp, bool renderChildren) override;

private:
    float GetCameraDistScale(const Vector3& position) const;

    void RenderComponentGizmosWhenNotSelected(GameObject *go);
    void RenderComponentGizmos(Component *comp, bool isBeingSelected);

    void RenderBoxColliderGizmo(BoxCollider *bc, bool isBeingSelected);
    void RenderCapsuleColliderGizmo(CapsuleCollider *cc, bool isBeingSelected);
    void RenderSphereColliderGizmo(SphereCollider *sc, bool isBeingSelected);
    void RenderCameraGizmo(Camera *cam, bool isBeingSelected);
    void RenderPointLightGizmo(PointLight *pointLight, bool isBeingSelected);
    void RenderDirectionalLightGizmo(DirectionalLight *dirLight,
                                     bool isBeingSelected);
    void RenderReflectionProbeGizmo(ReflectionProbe *reflProbe,
                                    bool isBeingSelected);
    void RenderParticleSystemGizmo(ParticleSystem *particleSystem,
                                   bool isBeingSelected);
    void RenderRopeGizmo(Rope *rope, bool isBeingSelected);
    void RenderAudioSourceGizmo(AudioSource *audioSource, bool isBeingSelected);
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTSGIZMOS_H

