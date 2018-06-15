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
    return ModelPreviewFactory::GetActive()->GetPreviewTextureFor_(model);
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

void ModelPreviewFactory::OnUpdateTextureBegin(Scene *previewScene,
                                               Camera *previewCamera,
                                               GameObject *previewGoContainer,
                                               Model *model)
{
    ASSERT(previewScene);

    GameObject *modelGo = model->CreateGameObjectFromModel();
    modelGo->SetName("ModelContainer");
    modelGo->SetParent(previewGoContainer);

    previewScene->Start();

    // Modify material
    List<MeshRenderer*> meshRenderers = previewGoContainer->
                                        GetComponentsInChildren<MeshRenderer>(true);
    for (MeshRenderer *mr : meshRenderers)
    {
        mr->GetMaterial()->SetCullFace(GL::CullFaceExt::NONE);
    }

    // Focus camera to model
    Transform *camTR = previewCamera->GetGameObject()->GetTransform();
    Sphere modelSphere = modelGo->GetBoundingSphere();
    float halfFov = Math::DegToRad(previewCamera->GetFovDegrees() / 2.0f);
    float camDist = modelSphere.GetRadius() / Math::Tan(halfFov) * 1.1f;
    Vector3 camDir = Vector3(1,1,-1).Normalized();
    camTR->SetPosition( modelSphere.GetCenter() + camDir * camDist);
    camTR->LookAt(modelSphere.GetCenter());

    previewCamera->SetZFar( (camDist +   modelSphere.GetRadius() * 2.0f) * 1.2f);
}

void ModelPreviewFactory::OnUpdateTextureEnd(Scene *previewScene,
                                             Camera *previewCamera,
                                             GameObject *previewGoContainer,
                                             Model *model)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;
    (void) model;

    GameObject *modelGo = previewGoContainer->FindInChildren("ModelContainer");
    ASSERT(modelGo);

    GameObject::Destroy(modelGo);
    previewScene->DestroyPending();
}

