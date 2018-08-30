#include "BangEditor/ModelPreviewFactory.h"

#include "Bang/Model.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorResources.h"

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
    return ModelPreviewFactory::GetActive()->GetPreviewTextureFor_(model,
                                                                   params);
}

ModelPreviewFactory *ModelPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetModelPreviewFactory();
}

void ModelPreviewFactory::OnCreateSceneFirstTime(Scene *previewScene,
                                                 Camera *previewCamera,
                                                 GameObject *previewGoContainer)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;
}

void ModelPreviewFactory::OnUpdateTextureBegin(
                                Scene *previewScene,
                                Camera *previewCamera,
                                GameObject *previewGoContainer,
                                Model *model,
                                const ResourcePreviewFactoryParameters &params)
{
    (void) previewCamera;
    (void) params;

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
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;
    (void) model;
    (void) params;

    GameObject *modelGo = previewGoContainer->FindInChildren("ModelContainer");
    ASSERT(modelGo);

    GameObject::DestroyImmediate(modelGo);
}

