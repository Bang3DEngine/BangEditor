#include "BangEditor/MeshPreviewFactory.h"

#include "Bang/Assert.h"
#include "Bang/GL.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/Material.h"
#include "Bang/Mesh.h"
#include "Bang/MeshRenderer.h"
#include "Bang/Scene.h"
#include "Bang/Texture2D.h"
#include "BangEditor/EditorResources.h"
#include "BangEditor/ResourcePreviewFactory.tcc"

namespace Bang
{
class Camera;
}

using namespace Bang;
using namespace BangEditor;

MeshPreviewFactory::MeshPreviewFactory()
{
}

MeshPreviewFactory::~MeshPreviewFactory()
{
}

RH<Texture2D> MeshPreviewFactory::GetPreviewTextureFor(Mesh *mesh)
{
    return MeshPreviewFactory::GetPreviewTextureFor(
        mesh, ResourcePreviewFactoryParameters());
}

RH<Texture2D> MeshPreviewFactory::GetPreviewTextureFor(
    Mesh *mesh,
    const ResourcePreviewFactoryParameters &params)
{
    return MeshPreviewFactory::GetActive()->GetPreviewTextureFor_(mesh, params);
}

MeshPreviewFactory *MeshPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetMeshPreviewFactory();
}

void MeshPreviewFactory::OnCreateSceneFirstTime(Scene *previewScene,
                                                Camera *previewCamera,
                                                GameObject *previewGoContainer)
{
    BANG_UNUSED_2(previewScene, previewCamera);

    GameObject *meshGo = GameObjectFactory::CreateGameObject();
    meshGo->SetName("MeshContainer");

    MeshRenderer *mr = meshGo->AddComponent<MeshRenderer>();
    mr->GetMaterial()->SetCullFace(GL::CullFaceExt::NONE);

    meshGo->SetParent(previewGoContainer);
}

void MeshPreviewFactory::OnUpdateTextureBegin(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer,
    Mesh *mesh,
    const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_3(previewScene, previewCamera, params);
    ASSERT(previewScene);

    GameObject *meshGo = previewScene->FindInChildren("MeshContainer");

    MeshRenderer *mr = meshGo->GetComponent<MeshRenderer>();
    mr->SetMesh(mesh);

    previewScene->Start();
}

void MeshPreviewFactory::OnUpdateTextureEnd(
    Scene *previewScene,
    Camera *previewCamera,
    GameObject *previewGoContainer,
    Mesh *mesh,
    const ResourcePreviewFactoryParameters &params)
{
    BANG_UNUSED_5(previewScene, previewCamera, previewGoContainer, mesh,
                  params);
}
