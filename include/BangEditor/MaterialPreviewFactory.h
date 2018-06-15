#ifndef MATERIALPREVIEWFACTORY_H
#define MATERIALPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/UMap.h"
#include "Bang/Material.h"
#include "Bang/Texture2D.h"
#include "Bang/Framebuffer.h"
#include "Bang/IEventsResource.h"

#include "BangEditor/ResourcePreviewFactory.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Camera;
FORWARD class MeshRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MaterialPreviewFactory : public ResourcePreviewFactory<Material>
{
public:
    MaterialPreviewFactory();
    virtual ~MaterialPreviewFactory();

    static RH<Texture2D> GetPreviewTextureFor(Material *material);
    static MaterialPreviewFactory *GetActive();

private:
    UMap<GUID, RH<Texture2D>> m_previewsMap;
    Array<RH<Material>> m_materials;

    Scene *m_previewScene = nullptr;
    Camera *p_previewCamera = nullptr;
    MeshRenderer *p_previewMeshRenderer = nullptr;
    Framebuffer *m_auxiliarFBToCopyTextures = nullptr;

    // ResourcePreviewFactory
    void OnCreateSceneFirstTime(Scene *scene) override;
    void OnUpdateTextureBegin(Scene *scene, Material *material) override;
    void OnUpdateTextureEnd(Scene *scene, Material *material) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MATERIALPREVIEWFACTORY_H

