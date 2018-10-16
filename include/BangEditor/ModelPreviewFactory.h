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
#include "Bang/ResourceHandle.h"
#include "Bang/UMap.tcc"
#include "Bang/Vector.tcc"
#include "BangEditor/BangEditor.h"
#include "BangEditor/ResourcePreviewFactory.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Camera;
FORWARD class GameObject;
FORWARD class Scene;
FORWARD class Texture2D;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ModelPreviewFactory : public ResourcePreviewFactory<Model>
{
public:
	ModelPreviewFactory();
	virtual ~ModelPreviewFactory() override;

    static RH<Texture2D> GetPreviewTextureFor(Model *model);
    static RH<Texture2D> GetPreviewTextureFor(
                                Model *model,
                                const ResourcePreviewFactoryParameters &params);
    static ModelPreviewFactory *GetActive();

private:
   // ResourcePreviewFactory
   void OnCreateSceneFirstTime(Scene *previewScene,
                               Camera *previewCamera,
                               GameObject *previewGoContainer) override;
   void OnUpdateTextureBegin(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Model *model,
                const ResourcePreviewFactoryParameters &params) override;
   void OnUpdateTextureEnd(
                Scene *previewScene,
                Camera *previewCamera,
                GameObject *previewGoContainer,
                Model *model,
                const ResourcePreviewFactoryParameters &params) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // MODELPREVIEWFACTORY_H

