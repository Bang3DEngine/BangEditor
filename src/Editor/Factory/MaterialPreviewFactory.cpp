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
    return MaterialPreviewFactory::GetActive()->GetPreviewTextureFor_(material);
}

MaterialPreviewFactory *MaterialPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetMaterialPreviewFactory();
}

void MaterialPreviewFactory::OnCreateSceneFirstTime(Scene *previewScene,
                                                    Camera *previewCamera,
                                                    GameObject *previewGoContainer)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;

    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->GetTransform()->SetPosition( Vector3::Zero );
    sphere->SetParent(previewGoContainer);
}

void MaterialPreviewFactory::OnUpdateTextureBegin(Scene *previewScene,
                                                  Camera *previewCamera,
                                                  GameObject *previewGoContainer,
                                                  Material *material)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;

    MeshRenderer *mr = previewGoContainer->
                       GetComponentInChildren<MeshRenderer>(true);
    ASSERT(mr);
    mr->SetMaterial(material);
}

void MaterialPreviewFactory::OnUpdateTextureEnd(Scene *previewScene,
                                                Camera *previewCamera,
                                                GameObject *previewGoContainer,
                                                Material *material)
{
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;
    (void) material;
}

