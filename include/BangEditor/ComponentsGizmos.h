#ifndef COMPONENTSGIZMOS_H
#define COMPONENTSGIZMOS_H

#include "Bang/GameObject.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Rope;
FORWARD class PointLight;
FORWARD class AudioSource;
FORWARD class BoxCollider;
FORWARD class ParticleSystem;
FORWARD class SphereCollider;
FORWARD class CapsuleCollider;
FORWARD class ReflectionProbe;
FORWARD class DirectionalLight;
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

