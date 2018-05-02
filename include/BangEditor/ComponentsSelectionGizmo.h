#ifndef COMPONENTSSELECTIONGIZMO_H
#define COMPONENTSSELECTIONGIZMO_H

#include "Bang/GameObject.h"
#include "BangEditor/SelectionGizmo.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class PointLight;
FORWARD class AudioSource;
FORWARD class DirectionalLight;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ComponentsSelectionGizmo : public SelectionGizmo
{
    GAMEOBJECT_EDITOR(ComponentsSelectionGizmo);

public:
    ComponentsSelectionGizmo();
    virtual ~ComponentsSelectionGizmo();

    // GameObject
    void Update() override;
    void Render(RenderPass rp, bool renderChildren) override;

    // SelectionGizmo
    void SetReferencedGameObject(GameObject *referencedGameObject) override;

private:
    void RenderCameraGizmo(Camera *cam);
    void RenderPointLightGizmo(PointLight *pointLight);
    void RenderDirectionalLightGizmo(DirectionalLight *dirLight);
    void RenderAudioSourceGizmo(AudioSource *audioSource);
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTSSELECTIONGIZMO_H

