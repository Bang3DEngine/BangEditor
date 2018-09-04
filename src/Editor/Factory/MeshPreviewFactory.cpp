#include "BangEditor/MeshPreviewFactory.h"

#include "Bang/Mesh.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/EditorResources.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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
    (void) previewScene;
    (void) previewCamera;

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
    (void) previewGoContainer;
    (void) previewCamera;
    (void) params;

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
    (void) previewScene;
    (void) previewCamera;
    (void) previewGoContainer;
    (void) mesh;
    (void) params;
}

