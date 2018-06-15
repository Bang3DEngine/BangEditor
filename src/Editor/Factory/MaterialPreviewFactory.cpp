#include "BangEditor/MaterialPreviewFactory.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/GLUniforms.h"
#include "Bang/Framebuffer.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/TextureFactory.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/ShaderProgramFactory.h"

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

void MaterialPreviewFactory::OnCreateSceneFirstTime(Scene *scene)
{
    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->GetTransform()->SetPosition( Vector3::Zero );
    sphere->SetParent(scene);

    p_previewMeshRenderer = sphere->GetComponent<MeshRenderer>();
}

void MaterialPreviewFactory::OnUpdateTextureBegin(Scene *scene,
                                                  Material *material)
{
    (void) scene;
    p_previewMeshRenderer->SetMaterial(material);
}

void MaterialPreviewFactory::OnUpdateTextureEnd(Scene *scene,
                                                Material *material)
{
    (void) scene;
    (void) material;
}

