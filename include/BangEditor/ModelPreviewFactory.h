#ifndef MODELPREVIEWFACTORY_H
#define MODELPREVIEWFACTORY_H

#include "Bang/Map.h"
#include "Bang/UMap.h"
#include "Bang/Model.h"
#include "Bang/Texture2D.h"
#include "Bang/Framebuffer.h"
#include "Bang/IEventsResource.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Scene;
FORWARD class Camera;
FORWARD class GameObject;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ModelPreviewFactory : public EventListener<IEventsResource>
{
public:
	ModelPreviewFactory();
	virtual ~ModelPreviewFactory();

    void Init();

    static RH<Texture2D> GetPreviewTextureFor(Model *model);

    static ModelPreviewFactory *GetActive();

private:
    UMap<GUID, RH<Texture2D>> m_previewsMap;
    Array<RH<Model>> m_models;

    Camera *p_previewCamera = nullptr;
    Map<GUID, Scene*> m_previewScenes;
    Framebuffer *m_auxiliarFBToCopyTextures = nullptr;

    void CreatePreviewSceneFor(Model *model);
    RH<Texture2D> GetPreviewTextureFor_(Model *model);
    void FillTextureWithPreview(Texture2D *texture, Model *model);

    // IEventsResource
    void OnResourceChanged(Resource *changedResource) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MODELPREVIEWFACTORY_H

