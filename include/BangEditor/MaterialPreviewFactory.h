#ifndef MATERIALPREVIEWFACTORY_H
#define MATERIALPREVIEWFACTORY_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Material.h"
#include "BangMath/Quaternion.h"
#include "Bang/UMap.tcc"
#include "BangEditor/AssetPreviewFactory.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Camera;
class GameObject;
class Scene;
class Texture2D;
}  // namespace Bang

using namespace Bang;
namespace BangEditor
{
class MaterialPreviewFactory : public AssetPreviewFactory<Material>
{
public:
    MaterialPreviewFactory();
    virtual ~MaterialPreviewFactory() override;

    static AH<Texture2D> GetPreviewTextureFor(Material *material);
    static AH<Texture2D> GetPreviewTextureFor(
        Material *material,
        const AssetPreviewFactoryParameters &params);
    static MaterialPreviewFactory *GetActive();

private:
    // AssetPreviewFactory
    void OnCreateSceneFirstTime(Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer) override;
    void OnUpdateTextureBegin(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Material *material,
        const AssetPreviewFactoryParameters &params) override;
    void OnUpdateTextureEnd(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Material *material,
        const AssetPreviewFactoryParameters &params) override;
};
}  // namespace BangEditor

#endif  // MATERIALPREVIEWFACTORY_H
