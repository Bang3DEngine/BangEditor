#ifndef COMPONENTSGIZMOS_H
#define COMPONENTSGIZMOS_H

#include "Bang/GameObject.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class PointLight;
FORWARD class AudioSource;
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
    void RenderGameObjectGizmosWhenNotSelected(GameObject *go);
    void RenderComponentGizmos(Component *comp, bool whenCompIsSelected);

    void RenderCameraGizmo(Camera *cam,
                           bool whenCompIsSelected);
    void RenderPointLightGizmo(PointLight *pointLight,
                               bool whenCompIsSelected);
    void RenderDirectionalLightGizmo(DirectionalLight *dirLight,
                                     bool whenCompIsSelected);
    void RenderReflectionProbeGizmo(ReflectionProbe *reflProbe,
                                    bool whenCompIsSelected);
    void RenderAudioSourceGizmo(AudioSource *audioSource,
                                bool whenCompIsSelected);
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTSGIZMOS_H

