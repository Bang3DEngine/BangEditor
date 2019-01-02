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
#include "BangEditor/AssetPreviewFactory.tcc"
#include "BangEditor/EditorAssets.h"
#include "BangEditor/EditorTextureFactory.h"

namespace Bang
{
class Camera;
}

using namespace Bang;
using namespace BangEditor;

ModelPreviewFactory::ModelPreviewFactory()
{
}

ModelPreviewFactory::~ModelPreviewFactory()
{
}

AH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor(Model *model)
{
    return ModelPreviewFactory::GetPreviewTextureFor(
        model, AssetPreviewFactoryParameters());
}

AH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor(
    Model *model,
    const AssetPreviewFactoryParameters &params)
{
    AH<Texture2D> texAH;
    if (model)
    {
        if (model->GetMeshes().Size() >= 1)
        {
            texAH = ModelPreviewFactory::GetActive()->GetPreviewTextureFor_(
                model, params);
        }
        else
        {
            texAH.Set(EditorTextureFactory::GetIconForExtension(
                Extensions::GetAnimationExtension()));
        }
    }
    return texAH;
}

ModelPreviewFactory *ModelPreviewFactory::GetActive()
{
    return EditorAssets::GetInstance()->GetModelPreviewFactory();
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
    const AssetPreviewFactoryParameters &params)
{
    BANG_UNUSED_2(previewCamera, params);

    ASSERT(previewScene);

    GameObject *modelGo = model->CreateGameObjectFromModel();
    modelGo->SetName("ModelContainer");
    modelGo->SetParent(previewGoContainer);

    previewScene->Start();

    // Modify material
    Array<MeshRenderer *> meshRenderers =
        previewGoContainer->GetComponentsInDescendantsAndThis<MeshRenderer>();
    for (MeshRenderer *mr : meshRenderers)
    {
        mr->GetMaterial()->GetShaderProgramProperties().SetCullFace(
            GL::CullFaceExt::NONE);
    }
}

void ModelPreviewFactory::OnUpdateTextureEnd(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer,
    Model *model,
    const AssetPreviewFactoryParameters &params)
{
    BANG_UNUSED_4(previewScene, previewCamera, model, params);

    GameObject *modelGo = previewGoContainer->FindInChildren("ModelContainer");
    ASSERT(modelGo);

    GameObject::DestroyImmediate(modelGo);
}
