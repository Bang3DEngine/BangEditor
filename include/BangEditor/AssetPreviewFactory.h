#ifndef ASSETPREVIEWFACTORY_H
#define ASSETPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/Framebuffer.h"
#include "Bang/IEventsAsset.h"
#include "Bang/Texture2D.h"
#include "Bang/UMap.h"

#include "BangEditor/BangEditor.h"

namespace Bang
{
class Scene;
class Camera;
}

using namespace Bang;
namespace BangEditor
{
struct AssetPreviewFactoryParameters
{
    Vector2 camOrbitAnglesDegs = Vector2(45.0f, -45.0f);
    float camDistanceMultiplier = 1.0f;

    bool operator==(const AssetPreviewFactoryParameters &rhs) const
    {
        return camOrbitAnglesDegs == rhs.camOrbitAnglesDegs &&
               camDistanceMultiplier == rhs.camDistanceMultiplier;
    }
    bool operator!=(const AssetPreviewFactoryParameters &rhs) const
    {
        return !((*this) == rhs);
    }
};

template <class T>
class AssetPreviewFactory : public EventListener<IEventsAsset>
{
public:
    virtual void Init();

protected:
    AssetPreviewFactory();
    virtual ~AssetPreviewFactory() override;

    virtual void OnCreateSceneFirstTime(Scene *previewScene,
                                        Camera *previewCamera,
                                        GameObject *previewGoContainer) = 0;
    virtual void OnUpdateTextureBegin(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        T *asset,
        const AssetPreviewFactoryParameters &params) = 0;
    virtual void OnUpdateTextureEnd(
        Scene *previewScene,
        Camera *previewCamera,
        GameObject *previewGoContainer,
        T *asset,
        const AssetPreviewFactoryParameters &params) = 0;

    Scene *GetPreviewScene() const;
    Camera *GetPreviewCamera() const;
    GameObject *GetPreviewGameObjectContainer() const;
    AH<Texture2D> GetPreviewTextureFor_(
        T *asset,
        const AssetPreviewFactoryParameters &params);

private:
    Array<AH<T>> m_previewsAssets;
    UMap<GUID, AH<Texture2D>> m_previewsMap;
    UMap<GUID, AssetPreviewFactoryParameters> m_lastPreviewParameters;

    Scene *m_previewScene = nullptr;
    Camera *p_previewCamera = nullptr;
    GameObject *p_previewGameObjectContainer = nullptr;
    Framebuffer *m_auxiliarFBToCopyTextures = nullptr;

    void CreatePreviewScene();
    void FillTextureWithPreview(Texture2D *texture,
                                T *asset,
                                const AssetPreviewFactoryParameters &params);

    // IEventsAsset
    void OnAssetChanged(Asset *changedAsset) override;
};
}

#include "BangEditor/AssetPreviewFactory.tcc"

#endif  // ASSETPREVIEWFACTORY_H
