#ifndef COMPONENTSGIZMOS_H
#define COMPONENTSGIZMOS_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/RenderPass.h"
#include "Bang/String.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SelectionGizmo.h"

namespace Bang
{
class AudioSource;
class BoxCollider;
class Camera;
class CapsuleCollider;
class Component;
class DirectionalLight;
class ParticleSystem;
class PointLight;
class ReflectionProbe;
class Rope;
class SphereCollider;
}

using namespace Bang;
namespace BangEditor
{
class ComponentsGizmos : public GameObject
{
    GAMEOBJECT_EDITOR(ComponentsGizmos);

public:
    ComponentsGizmos();
    virtual ~ComponentsGizmos() override;

    // GameObject
    void Render(RenderPass rp, bool renderChildren) override;

private:
    float GetCameraDistScale(const Vector3 &position) const;

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
}

#endif  // COMPONENTSGIZMOS_H
