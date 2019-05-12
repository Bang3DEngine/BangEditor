#ifndef MESHPREVIEWFACTORY_H
#define MESHPREVIEWFACTORY_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/AssetHandle.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.tcc"
#include "Bang/IEvents.h"
#include "Bang/Mesh.h"
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
class MeshPreviewFactory : public AssetPreviewFactory<Mesh>
{
public:
    MeshPreviewFactory();
    virtual ~MeshPreviewFactory() override;

    static AH<Texture2D> GetPreviewTextureFor(Mesh *Mesh);
    static AH<Texture2D> GetPreviewTextureFor(
        Mesh *mesh,
        const AssetPreviewFactoryParameters &params);
    static MeshPreviewFactory *GetActive();

private:
    // AssetPreviewFactory
    void OnCreateSceneFirstTime(Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer) override;
    void OnUpdateTextureBegin(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Mesh *mesh,
        const AssetPreviewFactoryParameters &params) override;
    void OnUpdateTextureEnd(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        Mesh *mesh,
        const AssetPreviewFactoryParameters &params) override;
};
}  // namespace BangEditor

#endif  // MESHPREVIEWFACTORY_H
