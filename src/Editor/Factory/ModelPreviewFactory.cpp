#include "BangEditor/ModelPreviewFactory.h"

#include "Bang/Model.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
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

    // Focus camera to model
    Transform *camTR = previewCamera->GetGameObject()->GetTransform();
    Sphere modelSphere = modelGo->GetBoundingSphere();
    float halfFov = Math::DegToRad(previewCamera->GetFovDegrees() / 2.0f);
    float camDist = modelSphere.GetRadius() / Math::Tan(halfFov) * 1.0f;
    camTR->SetPosition( modelSphere.GetCenter() - Vector3::Forward * camDist);
    camTR->LookAt(modelSphere.GetCenter());
}

void ModelPreviewFactory::OnUpdateTextureEnd(Scene *previewScene,
                                             Camera *previewCamera,
                                             GameObject *previewGoContainer,
                                             Model *model)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;

    GameObject *modelContainer = previewGoContainer->FindInChildren("ModelContainer");
    ASSERT(modelContainer);

    GameObject::Destroy(modelContainer);
}

