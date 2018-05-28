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

MaterialPreviewFactory::~MaterialPreviewFactory()
{
    if (m_previewScene) { GameObject::Destroy(m_previewScene); }

    for (RH<Material> mat : m_materials)
    {
        mat.Get()->EventEmitter<IMaterialChangedListener>::
                   UnRegisterListener(this);
    }
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor(Material *material)
{
    return MaterialPreviewFactory::GetActive()->GetPreviewTextureFor_(material);
}

RH<Texture2D> MaterialPreviewFactory::GetPreviewTextureFor_(Material *material)
{
    if (!material) { return TextureFactory::GetWhiteTexture(); }

    if (!m_previewsMap.ContainsKey(material->GetGUID()))
    {
        // Create empty preview texture
        RH<Texture2D> previewTexRH = Resources::Create<Texture2D>();
        Texture2D *previewTex = previewTexRH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, material);

        // Add to preview map
        material->EventEmitter<IMaterialChangedListener>::RegisterListener(this);
        m_previewsMap.Add(material->GetGUID(), previewTexRH);
        m_materials.PushBack( RH<Material>(material) );
    }

    return m_previewsMap.Get(material->GetGUID());
}

void MaterialPreviewFactory::CreatePreviewScene()
{
    Scene *scene = GameObjectFactory::CreateScene();

    GameObject *camGo = GameObjectFactory::CreateGameObject();
    camGo->GetTransform()->SetPosition( Vector3(1.3f) );
    camGo->GetTransform()->LookAt(Vector3::Zero);
    camGo->SetParent(scene);

    Camera *camera = camGo->AddComponent<Camera>();
    camera->SetClearMode(Camera::ClearMode::SKY_BOX);
    camera->SetSkyBoxTexture( TextureFactory::GetDefaultTextureCubeMap().Get() );
    scene->SetCamera(camera);

    GameObject *dLightGo = GameObjectFactory::CreateGameObject();
    DirectionalLight *dLight = dLightGo->AddComponent<DirectionalLight>();
    dLight->SetShadowType(Light::ShadowType::NONE);
    dLightGo->SetParent(scene);

    GameObject *sphere = GameObjectFactory::CreateSphereGameObject();
    sphere->GetTransform()->SetPosition( Vector3::Zero );
    sphere->SetParent(scene);

    scene->Start();

    m_previewScene = scene;
    p_previewCamera = camera;
    p_previewMeshRenderer = sphere->GetComponent<MeshRenderer>();
}

MaterialPreviewFactory *MaterialPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetMaterialPreviewFactory();
}

void MaterialPreviewFactory::FillTextureWithPreview(Texture2D *texture,
                                                    Material *material)
{
    // Now we will fill the texture with the proper preview
    GL::Push(GL::Pushable::VIEWPORT);
    GL::Push(GL::BindTarget::TEXTURE_2D);
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    // Prepare to draw scene
    if (!m_previewScene) { CreatePreviewScene(); }
    ASSERT(m_previewScene);
    ASSERT(p_previewCamera);
    ASSERT(p_previewMeshRenderer);

    const uint previewTextureSize = 256;
    p_previewCamera->GetGBuffer()->SetAttachmentTexture(texture,
                                                        GL::Attachment::COLOR0);
    p_previewCamera->GetGBuffer()->Resize(previewTextureSize, previewTextureSize);
    p_previewMeshRenderer->SetMaterial(material);

    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(m_previewScene, p_previewCamera);

    texture->Bind();
    texture->SetFilterMode(GL::FilterMode::BILINEAR);
    // texture->GenerateMipMaps();
    // texture->SetFilterMode(GL::FilterMode::Trilinear_LL);
    texture->PropagateTextureChanged();

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

void MaterialPreviewFactory::OnMaterialChanged(Material *changedMaterial)
{
    ASSERT(m_previewsMap.ContainsKey(changedMaterial->GetGUID()));

    SetReceiveEvents(false);

    Texture2D *matPreviewTex = m_previewsMap.Get(changedMaterial->GetGUID()).Get();
    FillTextureWithPreview(matPreviewTex, changedMaterial); // Update preview

    SetReceiveEvents(true);
}
