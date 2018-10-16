#ifndef MESHPREVIEWFACTORY_H
#define MESHPREVIEWFACTORY_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Mesh.h"
#include "Bang/Quaternion.h"
#include "Bang/ResourceHandle.h"
#include "Bang/UMap.tcc"
#include "Bang/Vector.tcc"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

namespace Bang {
class Camera;
class GameObject;
class Scene;
class Texture2D;
}  // namespace Bang

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

