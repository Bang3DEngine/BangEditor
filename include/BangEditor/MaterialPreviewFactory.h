#ifndef MATERIALPREVIEWFACTORY_H
#define MATERIALPREVIEWFACTORY_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Material.h"
#include "Bang/Quaternion.h"
#include "Bang/ResourceHandle.h"
#include "Bang/UMap.tcc"
#include "Bang/Vector.tcc"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

namespace Bang
{
class Camera;
class GameObject;
class Scene;
class Texture2D;
}

using namespace Bang;
namespace BangEditor
{
class MaterialPreviewFactory : public ResourcePreviewFactory<Material>
{
public:
    MaterialPreviewFactory();
    virtual ~MaterialPreviewFactory() override;

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
}

#endif  // MATERIALPREVIEWFACTORY_H
