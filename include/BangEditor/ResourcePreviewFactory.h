#ifndef RESOURCEPREVIEWFACTORY_H
#define RESOURCEPREVIEWFACTORY_H

#include "Bang/Bang.h"
#include "Bang/UMap.h"
#include "Bang/Texture2D.h"
#include "Bang/Framebuffer.h"
#include "Bang/IEventsResource.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Camera;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

struct ResourcePreviewFactoryParameters
{
    Vector2 camOrbitAnglesDegs = Vector2(45.0f, -45.0f);
    float camDistanceMultiplier = 1.0f;

    bool operator==(const ResourcePreviewFactoryParameters &rhs) const
    {
        return camOrbitAnglesDegs == rhs.camOrbitAnglesDegs &&
               camDistanceMultiplier == rhs.camDistanceMultiplier;
    }
    bool operator!=(const ResourcePreviewFactoryParameters &rhs) const
    {
        return !((*this) == rhs);
    }
};


template <class T>
class ResourcePreviewFactory : public EventListener<IEventsResource>
{
public:
    virtual void Init();

protected:
    ResourcePreviewFactory();
    virtual ~ResourcePreviewFactory();

    virtual void OnCreateSceneFirstTime(Scene *previewScene,
                                        Camera *previewCamera,
                                        GameObject *previewGoContainer) = 0;
    virtual void OnUpdateTextureBegin(
                         Scene *previewScene,
                         Camera *previewCamera,
                         GameObject *previewGoContainer,
                         T *resource,
                         const ResourcePreviewFactoryParameters &params) = 0;
    virtual void OnUpdateTextureEnd(
                         Scene *previewScene,
                         Camera *previewCamera,
                         GameObject *previewGoContainer,
                         T *resource,
                         const ResourcePreviewFactoryParameters &params) = 0;

    Scene *GetPreviewScene() const;
    Camera *GetPreviewCamera() const;
    GameObject *GetPreviewGameObjectContainer() const;
    RH<Texture2D> GetPreviewTextureFor_(
                            T *resource,
                            const ResourcePreviewFactoryParameters &params);

private:
    Array<RH<T>> m_previewsResources;
    UMap<GUID, RH<Texture2D>> m_previewsMap;
    UMap<GUID, ResourcePreviewFactoryParameters> m_lastPreviewParameters;

    Scene *m_previewScene = nullptr;
    Camera *p_previewCamera = nullptr;
    GameObject *p_previewGameObjectContainer = nullptr;
    Framebuffer *m_auxiliarFBToCopyTextures = nullptr;

    void CreatePreviewScene();
    void FillTextureWithPreview(
                            Texture2D *texture,
                            T *resource,
                            const ResourcePreviewFactoryParameters &params);

    // IEventsResource
    void OnResourceChanged(Resource *changedResource) override;
};

NAMESPACE_BANG_EDITOR_END

#include "BangEditor/ResourcePreviewFactory.tcc"

#endif // RESOURCEPREVIEWFACTORY_H

