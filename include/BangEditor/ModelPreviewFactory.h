#ifndef MODELPREVIEWFACTORY_H
#define MODELPREVIEWFACTORY_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Model.h"
#include "Bang/Quaternion.h"
#include "Bang/AssetHandle.h"
#include "Bang/UMap.tcc"
#include "BangEditor/BangEditor.h"
#include "BangEditor/AssetPreviewFactory.h"

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
class ModelPreviewFactory : public AssetPreviewFactory<Model>
{
public:
    ModelPreviewFactory();
    virtual ~ModelPreviewFactory() override;

    static AH<Texture2D> GetPreviewTextureFor(Model *model);
    static AH<Texture2D> GetPreviewTextureFor(
        Model *model,
        const AssetPreviewFactoryParameters &params);
    static ModelPreviewFactory *GetActive();

private:
    // AssetPreviewFactory
    void OnCreateSceneFirstTime(Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer) override;
    void OnUpdateTextureBegin(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Model *model,
        const AssetPreviewFactoryParameters &params) override;
    void OnUpdateTextureEnd(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Model *model,
        const AssetPreviewFactoryParameters &params) override;
};
}  // namespace BangEditor

#endif  // MODELPREVIEWFACTORY_H
