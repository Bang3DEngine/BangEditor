#ifndef MATERIALPREVIEWFACTORY_H
#define MATERIALPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/Material.h"

#include "BangEditor/ResourcePreviewFactory.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MaterialPreviewFactory : public ResourcePreviewFactory<Material>
{
public:
    MaterialPreviewFactory();
    virtual ~MaterialPreviewFactory();

    static RH<Texture2D> GetPreviewTextureFor(Material *material);
    static RH<Texture2D> GetPreviewTextureFor(
                              Material *material,
                              const ResourcePreviewFactoryParameters &params);
    static MaterialPreviewFactory *GetActive();

private:
    // ResourcePreviewFactory
    void OnCreateSceneFirstTime(Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer) override;
    void OnUpdateTextureBegin(
                  Scene *previewScene,
                  Camera *previewCamera,
                  GameObject *previewGoContainer,
                  Material *material,
                  const ResourcePreviewFactoryParameters &params) override;
    void OnUpdateTextureEnd(
                  Scene *previewScene,
                  Camera *previewCamera,
                  GameObject *previewGoContainer,
                  Material *material,
                  const ResourcePreviewFactoryParameters &params) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MATERIALPREVIEWFACTORY_H

