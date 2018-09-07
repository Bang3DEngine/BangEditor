#include "BangEditor/MaterialPreviewFactory.h"

#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorResources.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

MaterialPreviewFactory::MaterialPreviewFactory()
{
}

MaterialPreviewFactory::~MaterialPreviewFactory()
{
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(Material *material)
{
    return MaterialPreviewFactory::GetPreviewTextureFor(
                                        material,
                                        ResourcePreviewFactoryParameters());
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(
                            Material *material,
                            const ResourcePreviewFactoryParameters &params)
{
    return MaterialPreviewFactory::GetActive()->GetPreviewTextureFor_(material,
                                                                      params);
}

MaterialPreviewFactory *MaterialPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetMaterialPreviewFactory();
}

void MaterialPreviewFactory::OnCreateSceneFirstTime(Scene *previewScene,
                                                    Camera *previewCamera,
                                                    GameObject *previewGoContainer)
{
    BANG_UNUSED_3(previewScene, previewCamera, previewGoContainer);

    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->SetParent(previewGoContainer);
    sphere->GetTransform()->SetPosition( Vector3::Zero );
}

void MaterialPreviewFactory::OnUpdateTextureBegin(
                              Scene *previewScene,
                              Camera *previewCamera,
                              GameObject *previewGoContainer,
                              Material *material,
                              const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_4(params, previewScene, previewCamera, previewGoContainer);

    MeshRenderer *mr = previewGoContainer->
                       GetComponentInChildren<MeshRenderer>(true);
    ASSERT(mr);
    mr->SetMaterial(material);
}

void MaterialPreviewFactory::OnUpdateTextureEnd(
                              Scene *previewScene,
                              Camera *previewCamera,
                              GameObject *previewGoContainer,
                              Material *material,
                              const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_5(previewScene, previewCamera,
                  previewGoContainer, material, params);
}

