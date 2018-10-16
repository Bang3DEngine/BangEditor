#include "BangEditor/ModelPreviewFactory.h"

#include "Bang/Array.h"
#include "Bang/Assert.h"
#include "Bang/Extensions.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/Material.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Model.h"
#include "Bang/Scene.h"
#include "Bang/Texture2D.h"
#include "BangEditor/EditorResources.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/ResourcePreviewFactory.tcc"

namespace Bang {
class Camera;
}  // namespace Bang

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ModelPreviewFactory::ModelPreviewFactory()
{
}

ModelPreviewFactory::~ModelPreviewFactory()
{
}

RH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor(Model *model)
{
    return ModelPreviewFactory::GetPreviewTextureFor(
                            model, ResourcePreviewFactoryParameters());
}

RH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor(
                                Model *model,
                                const ResourcePreviewFactoryParameters &params)
{
    RH<Texture2D> texRH;
    if (model)
    {
        if (model->GetMeshes().Size() >= 1)
        {
            texRH = ModelPreviewFactory::GetActive()->
                                         GetPreviewTextureFor_(model, params);
        }
        else
        {
            texRH.Set(EditorTextureFactory::GetIconForExtension(
                                Extensions::GetAnimationExtension() ) );
        }
    }
    return texRH;
}

ModelPreviewFactory *ModelPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetModelPreviewFactory();
}

void ModelPreviewFactory::OnCreateSceneFirstTime(Scene *previewScene,
                                                 Camera *previewCamera,
                                                 GameObject *previewGoContainer)
{
    BANG_UNUSED_3(previewScene, previewCamera, previewGoContainer);
}

void ModelPreviewFactory::OnUpdateTextureBegin(
                                Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer,
                                Model *model,
                                const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_2(previewCamera, params);

    ASSERT(previewScene);

    GameObject *modelGo = model->CreateGameObjectFromModel();
    modelGo->SetName("ModelContainer");
    modelGo->SetParent(previewGoContainer);

    previewScene->Start();

    // Modify material
    Array<MeshRenderer*> meshRenderers =
         previewGoContainer->GetComponentsInDescendantsAndThis<MeshRenderer>();
    for (MeshRenderer *mr : meshRenderers)
    {
        mr->GetMaterial()->SetCullFace(GL::CullFaceExt::NONE);
    }
}

void ModelPreviewFactory::OnUpdateTextureEnd(
                                Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer,
                                Model *model,
                                const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_4(previewScene, previewCamera, model, params);

    GameObject *modelGo = previewGoContainer->FindInChildren("ModelContainer");
    ASSERT(modelGo);

    GameObject::DestroyImmediate(modelGo);
}

