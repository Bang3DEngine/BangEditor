#ifndef RESOURCEPREVIEWFACTORY_TCC
#define RESOURCEPREVIEWFACTORY_TCC

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
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

#include "BangEditor/ResourcePreviewFactory.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

template <class T>
ResourcePreviewFactory<T>::ResourcePreviewFactory()
{
}

template <class T>
ResourcePreviewFactory<T>::~ResourcePreviewFactory()
{
    delete m_auxiliarFBToCopyTextures;

    if (m_previewScene)
    {
        GameObject::Destroy(m_previewScene);
    }

    for (RH<T> &res : m_previewsResources)
    {
        res.Get()->EventEmitter<IEventsResource>::UnRegisterListener(this);
    }
}

template<class T>
void ResourcePreviewFactory<T>::Init()
{
    m_auxiliarFBToCopyTextures = new Framebuffer();
    m_auxiliarFBToCopyTextures->CreateAttachmentTex2D(GL::Attachment::COLOR0,
                                                      GL::ColorFormat::RGBA8);
}

template<class T>
void ResourcePreviewFactory<T>::CreatePreviewScene()
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

    OnCreateSceneFirstTime(scene);

    scene->Start();

    m_previewScene = scene;
    p_previewCamera = camera;
}

template<class T>
RH<Texture2D> ResourcePreviewFactory<T>::GetPreviewTextureFor_(T *resource)
{
    if (!resource) { return TextureFactory::GetWhiteTexture(); }
    if (!m_previewsMap.ContainsKey(resource->GetGUID()))
    {
        // Create empty preview texture
        RH<Texture2D> previewTexRH = Resources::Create<Texture2D>();
        Texture2D *previewTex = previewTexRH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, resource);

        // Add to preview map
        resource->EventEmitter<IEventsResource>::RegisterListener(this);
        m_previewsMap.Add(resource->GetGUID(), previewTexRH);
        m_previewsResources.PushBack( RH<T>(resource) );
    }
    return m_previewsMap.Get(resource->GetGUID());
}

template<class T>
void ResourcePreviewFactory<T>::FillTextureWithPreview(Texture2D *texture,
                                                       T *resource)
{
    // Now we will fill the texture with the proper preview
    GL::Push(GL::Pushable::VIEWPORT);
    GL::Push(GL::BindTarget::TEXTURE_2D);
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    // Prepare to draw scene
    if (!m_previewScene) { CreatePreviewScene(); }
    ASSERT(m_previewScene);
    ASSERT(p_previewCamera);

    const uint previewTextureSize = 256;
    m_auxiliarFBToCopyTextures->SetAttachmentTexture(texture,
                                                     GL::Attachment::COLOR0);
    m_auxiliarFBToCopyTextures->Resize(previewTextureSize, previewTextureSize);

    p_previewCamera->GetGBuffer()->Resize(previewTextureSize, previewTextureSize);

    OnUpdateTextureBegin(m_previewScene, resource);

    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(m_previewScene, p_previewCamera);

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

    OnUpdateTextureEnd(m_previewScene, resource);

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

template<class T>
void ResourcePreviewFactory<T>::OnResourceChanged(Resource *changedResource)
{
    ASSERT(m_previewsMap.ContainsKey(changedResource->GetGUID()));

    SetReceiveEvents(false);

    T *changedResT = SCAST<T*>(changedResource);
    Texture2D *resPreviewTex = m_previewsMap.Get(changedResT->GetGUID()).Get();
    FillTextureWithPreview(resPreviewTex, changedResT); // Update preview

    SetReceiveEvents(true);
}

template<class T>
Scene *ResourcePreviewFactory<T>::GetPreviewScene() const
{
    return m_previewScene;
}

template<class T>
Camera *ResourcePreviewFactory<T>::GetPreviewCamera() const
{
    return p_previewCamera;
}

NAMESPACE_BANG_EDITOR_END

#endif // PREVIEWFACTORY_H

