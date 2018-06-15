#include "BangEditor/ModelPreviewFactory.h"

#include "Bang/GL.h"
#include "Bang/Model.h"
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

ModelPreviewFactory::ModelPreviewFactory()
{
}

ModelPreviewFactory::~ModelPreviewFactory()
{
    delete m_auxiliarFBToCopyTextures;

    for (auto &pair : m_previewScenes)
    {
        Scene *scene = pair.second;
        GameObject::Destroy(scene);
    }

    for (RH<Model> &model : m_models)
    {
        model.Get()->EventEmitter<IEventsResource>::UnRegisterListener(this);
    }
}

void ModelPreviewFactory::Init()
{
    m_auxiliarFBToCopyTextures = new Framebuffer();
    m_auxiliarFBToCopyTextures->CreateAttachmentTex2D(GL::Attachment::COLOR0,
                                                      GL::ColorFormat::RGBA8);
}

RH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor(Model *model)
{
    return ModelPreviewFactory::GetActive()->GetPreviewTextureFor_(model);
}

ModelPreviewFactory *ModelPreviewFactory::GetActive()
{
    return EditorResources::GetInstance()->GetModelPreviewFactory();
}

void ModelPreviewFactory::CreatePreviewSceneFor(Model *model)
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

    scene->Start();

    p_previewCamera = camera;
    m_previewScenes.Add(model->GetGUID(), scene);
}

RH<Texture2D> ModelPreviewFactory::GetPreviewTextureFor_(Model *model)
{
    if (!model) { return TextureFactory::GetWhiteTexture(); }
    if (!m_previewsMap.ContainsKey(model->GetGUID()))
    {
        // Create empty preview texture
        RH<Texture2D> previewTexRH = Resources::Create<Texture2D>();
        Texture2D *previewTex = previewTexRH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, model);

        // Add to preview map
        model->EventEmitter<IEventsResource>::RegisterListener(this);
        m_previewsMap.Add(model->GetGUID(), previewTexRH);
        m_models.PushBack( RH<Model>(model) );
    }

    return m_previewsMap.Get(model->GetGUID());
}

void ModelPreviewFactory::FillTextureWithPreview(Texture2D *texture,
                                                 Model *model)
{
    ASSERT(model);
    if (!m_previewScenes.ContainsKey(model->GetGUID()))
    {
        CreatePreviewSceneFor(model);
    }
    ASSERT(m_previewScenes.ContainsKey(model->GetGUID()));

    // Now we will fill the texture with the proper preview
    GL::Push(GL::Pushable::VIEWPORT);
    GL::Push(GL::BindTarget::TEXTURE_2D);
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    // Prepare to draw scene
    Scene *previewScene = m_previewScenes.Get(model->GetGUID());
    ASSERT(previewScene);
    ASSERT(p_previewCamera);

    // Create go from model every time we regenerate texture,
    // to catch changes if any
    GameObject *modelGo = model->CreateGameObjectFromModel();
    modelGo->SetParent(previewScene);

    previewScene->Start();

    // Focus camera to model
    // Transform *modelTR = modelGo->GetTransform();
    Transform *camTR = p_previewCamera->GetGameObject()->GetTransform();
    Sphere modelSphere = modelGo->GetBoundingSphere();
    float halfFov = Math::DegToRad(p_previewCamera->GetFovDegrees() / 2.0f);
    float camDist = modelSphere.GetRadius() / Math::Tan(halfFov) * 1.0f;
    camTR->SetPosition( modelSphere.GetCenter() - Vector3::Forward * camDist);
    camTR->LookAt(modelSphere.GetCenter());


    const uint previewTextureSize = 256;
    m_auxiliarFBToCopyTextures->SetAttachmentTexture(texture,
                                                     GL::Attachment::COLOR0);
    m_auxiliarFBToCopyTextures->Resize(previewTextureSize, previewTextureSize);

    p_previewCamera->GetGBuffer()->Resize(previewTextureSize, previewTextureSize);

    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(previewScene, p_previewCamera);

    GL::Bind(GL::BindTarget::READ_FRAMEBUFFER, p_previewCamera->GetGBuffer()->GetGLId());
    GL::Bind(GL::BindTarget::DRAW_FRAMEBUFFER, m_auxiliarFBToCopyTextures->GetGLId());

    GL::ReadBuffer(p_previewCamera->GetGBuffer()->GetLastDrawnColorAttachment());
    GL::DrawBuffers( {GL::Attachment::COLOR0} );

    GL::BlitFramebuffer(GL::GetViewportRect(),
                        GL::GetViewportRect(),
                        GL::FilterMode::NEAREST,
                        GL::BufferBit::COLOR);

    texture->Bind();
    texture->SetFilterMode(GL::FilterMode::BILINEAR);
    // texture->GenerateMipMaps();
    // texture->SetFilterMode(GL::FilterMode::Trilinear_LL);
    texture->PropagateResourceChanged();

    GameObject::Destroy(modelGo);

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

void ModelPreviewFactory::OnResourceChanged(Resource *changedResource)
{
    IEventsResource::OnResourceChanged(changedResource);

    ASSERT(m_previewsMap.ContainsKey(changedResource->GetGUID()));

    SetReceiveEvents(false);

    Model *changedModel = SCAST<Model*>(changedResource);
    Texture2D *matPreviewTex = m_previewsMap.Get(changedModel->GetGUID()).Get();
    FillTextureWithPreview(matPreviewTex, changedModel); // Update preview

    SetReceiveEvents(true);
}

