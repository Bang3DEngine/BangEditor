#include "BangEditor/MaterialPreviewFactory.h"

#include "Bang/Assert.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Texture2D.h"
#include "Bang/Transform.h"
#include "BangEditor/EditorAssets.h"
#include "BangEditor/AssetPreviewFactory.tcc"

namespace Bang
{
class Camera;
class Scene;
}

using namespace Bang;
using namespace BangEditor;

MaterialPreviewFactory::MaterialPreviewFactory()
{
}

MaterialPreviewFactory::~MaterialPreviewFactory()
{
}

AH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(Material *material)
{
    return MaterialPreviewFactory::GetPreviewTextureFor(
        material, AssetPreviewFactoryParameters());
}

AH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(
    Material *material,
    const AssetPreviewFactoryParameters &params)
{
    return MaterialPreviewFactory::GetActive()->GetPreviewTextureFor_(material,
                                                                      params);
}

MaterialPreviewFactory *MaterialPreviewFactory::GetActive()
{
    return EditorAssets::GetInstance()->GetMaterialPreviewFactory();
}

void MaterialPreviewFactory::OnCreateSceneFirstTime(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer)
{
    BANG_UNUSED_3(previewScene, previewCamera, previewGoContainer);

    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->SetParent(previewGoContainer);
    sphere->GetTransform()->SetPosition(Vector3::Zero());
}

void MaterialPreviewFactory::OnUpdateTextureBegin(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer,
    Material *material,
    const AssetPreviewFactoryParameters &params)
{
    BANG_UNUSED_4(params, previewScene, previewCamera, previewGoContainer);

    MeshRenderer *mr =
        previewGoContainer->GetComponentInDescendants<MeshRenderer>();
    ASSERT(mr);
    mr->SetMaterial(material);
}

void MaterialPreviewFactory::OnUpdateTextureEnd(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer,
    Material *material,
    const AssetPreviewFactoryParameters &params)
{
    BANG_UNUSED_5(
        previewScene, previewCamera, previewGoContainer, material, params);
}
