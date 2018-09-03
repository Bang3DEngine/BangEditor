#ifndef MESHPREVIEWFACTORY_H
#define MESHPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/Mesh.h"

#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class MeshPreviewFactory : public ResourcePreviewFactory<Mesh>
{
public:
    MeshPreviewFactory();
    virtual ~MeshPreviewFactory();

    static RH<Texture2D> GetPreviewTextureFor(Mesh *Mesh);
    static RH<Texture2D> GetPreviewTextureFor(
                                Mesh *mesh,
                                const ResourcePreviewFactoryParameters &params);
    static MeshPreviewFactory *GetActive();

private:
   // ResourcePreviewFactory
   void OnCreateSceneFirstTime(Scene *previewScene,
                               Camera *previewCamera,
                               GameObject *previewGoContainer) override;
   void OnUpdateTextureBegin(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Mesh *mesh,
                const ResourcePreviewFactoryParameters &params) override;
   void OnUpdateTextureEnd(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Mesh *mesh,
                const ResourcePreviewFactoryParameters &params) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MESHPREVIEWFACTORY_H

