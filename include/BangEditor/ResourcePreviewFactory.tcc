#pragma once

#include "Bang/Camera.h"
#include "Bang/DirectionalLight.h"
#include "Bang/Framebuffer.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GL.h"
#include "Bang/GLUniforms.h"
#include "Bang/GameObject.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/ResourceHandle.h"
#include "Bang/Resources.h"
#include "Bang/Scene.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Sphere.h"
#include "Bang/TextureFactory.h"
#include "Bang/Transform.h"

#include "BangEditor/ResourcePreviewFactory.h"

using namespace Bang;
namespace BangEditor
{
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
        GameObject::DestroyImmediate(m_previewScene);
    }

    for (RH<T> &res : m_previewsResources)
    {
        res.Get()->EventEmitter<IEventsResource>::UnRegisterListener(this);
    }
}

template <class T>
void ResourcePreviewFactory<T>::Init()
{
    m_auxiliarFBToCopyTextures = new Framebuffer();
    m_auxiliarFBToCopyTextures->CreateAttachmentTex2D(GL::Attachment::COLOR0,
                                                      GL::ColorFormat::RGBA8);
}

template <class T>
void ResourcePreviewFactory<T>::CreatePreviewScene()
{
    Scene *scene = GameObjectFactory::CreateScene();

    GameObject *camGo = GameObjectFactory::CreateGameObject();
    camGo->GetTransform()->SetPosition(Vector3(1.3f));
    camGo->GetTransform()->LookAt(Vector3::Zero());
    camGo->SetParent(scene);

    Camera *previewCamera = camGo->AddComponent<Camera>();
    previewCamera->SetClearMode(CameraClearMode::SKY_BOX);
    previewCamera->SetSkyBoxTexture(TextureFactory::GetDefaultSkybox());
    scene->SetCamera(previewCamera);

    GameObject *dLightGo = GameObjectFactory::CreateGameObject();
    DirectionalLight *dLight = dLightGo->AddComponent<DirectionalLight>();
    dLight->SetShadowType(Light::ShadowType::NONE);
    dLightGo->SetParent(scene);

    GameObject *previewGameObjectContainer =
        GameObjectFactory::CreateGameObject();
    previewGameObjectContainer->SetParent(scene);

    OnCreateSceneFirstTime(scene, previewCamera, previewGameObjectContainer);

    scene->Start();

    m_previewScene = scene;
    p_previewCamera = previewCamera;
    p_previewGameObjectContainer = previewGameObjectContainer;
}

template <class T>
RH<Texture2D> ResourcePreviewFactory<T>::GetPreviewTextureFor_(
    T *resource,
    const ResourcePreviewFactoryParameters &params)
{
    if (!resource)
    {
        return RH<Texture2D>(TextureFactory::GetWhiteTexture());
    }

    if (!m_lastPreviewParameters.ContainsKey(resource->GetGUID()) ||
        params != m_lastPreviewParameters.Get(resource->GetGUID()))
    {
        m_previewsMap.Remove(resource->GetGUID());
    }

    if (!m_previewsMap.ContainsKey(resource->GetGUID()))
    {
        // Create empty preview texture
        RH<Texture2D> previewTexRH = Resources::Create<Texture2D>();
        Texture2D *previewTex = previewTexRH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, resource, params);

        // Add to preview map
        resource->EventEmitter<IEventsResource>::RegisterListener(this);
        m_previewsMap.Add(resource->GetGUID(), previewTexRH);
        m_previewsResources.PushBack(RH<T>(resource));
    }
    return m_previewsMap.Get(resource->GetGUID());
}

template <class T>
void ResourcePreviewFactory<T>::FillTextureWithPreview(
    Texture2D *texture,
    T *resource,
    const ResourcePreviewFactoryParameters &params)
{
    // Now we will fill the texture with the proper preview
    GL::Push(GL::Pushable::VIEWPORT);
    GL::Push(GL::BindTarget::TEXTURE_2D);
    GL::Push(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);

    // Prepare to draw scene
    if (!GetPreviewScene())
    {
        CreatePreviewScene();
    }
    ASSERT(GetPreviewScene());
    ASSERT(GetPreviewCamera());

    const uint previewTextureSize = 256;
    m_auxiliarFBToCopyTextures->SetAttachmentTexture(texture,
                                                     GL::Attachment::COLOR0);
    m_auxiliarFBToCopyTextures->Resize(previewTextureSize, previewTextureSize);

    GetPreviewCamera()->GetGBuffer()->Resize(previewTextureSize,
                                             previewTextureSize);

    m_lastPreviewParameters.Add(resource->GetGUID(), params);
    OnUpdateTextureBegin(GetPreviewScene(),
                         GetPreviewCamera(),
                         GetPreviewGameObjectContainer(),
                         resource,
                         params);

    // Set camera
    Transform *camTR = GetPreviewCamera()->GetGameObject()->GetTransform();
    Sphere goSphere = GetPreviewGameObjectContainer()->GetBoundingSphere();
    float halfFov = Math::DegToRad(GetPreviewCamera()->GetFovDegrees() / 2.0f);
    float camDist = goSphere.GetRadius() / Math::Tan(halfFov) * 1.1f;
    camDist *= params.camDistanceMultiplier;
    const Vector2 &angles = params.camOrbitAnglesDegs;
    Vector3 camDir =
        (Quaternion::AngleAxis(Math::DegToRad(-angles.x), Vector3::Up()) *
         Quaternion::AngleAxis(Math::DegToRad(angles.y), Vector3::Right()) *
         Vector3(0, 0, 1))
            .Normalized();
    camTR->SetPosition(goSphere.GetCenter() + camDir * camDist);
    camTR->LookAt(goSphere.GetCenter());
    GetPreviewCamera()->SetZFar((camDist + goSphere.GetRadius() * 2.0f) * 1.2f);
    /*
    */

    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(GetPreviewScene(), GetPreviewCamera());

    GL::Bind(GL::BindTarget::READ_FRAMEBUFFER,
             GetPreviewCamera()->GetGBuffer()->GetGLId());
    GL::Bind(GL::BindTarget::DRAW_FRAMEBUFFER,
             m_auxiliarFBToCopyTextures->GetGLId());

    GL::ReadBuffer(
        GetPreviewCamera()->GetGBuffer()->GetLastDrawnColorAttachment());
    GL::DrawBuffers({GL::Attachment::COLOR0});

    GL::BlitFramebuffer(GL::GetViewportRect(),
                        GL::GetViewportRect(),
                        GL::FilterMode::NEAREST,
                        GL::BufferBit::COLOR);

    texture->Bind();
    texture->SetFilterMode(GL::FilterMode::BILINEAR);
    // texture->GenerateMipMaps();
    // texture->SetFilterMode(GL::FilterMode::Trilinear_LL);
    texture->PropagateResourceChanged();

    OnUpdateTextureEnd(GetPreviewScene(),
                       GetPreviewCamera(),
                       GetPreviewGameObjectContainer(),
                       resource,
                       params);

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

template <class T>
void ResourcePreviewFactory<T>::OnResourceChanged(Resource *changedResource)
{
    ASSERT(m_previewsMap.ContainsKey(changedResource->GetGUID()));

    SetReceiveEvents(false);

    T *changedResT = SCAST<T *>(changedResource);
    Texture2D *resPreviewTex = m_previewsMap.Get(changedResT->GetGUID()).Get();

    ASSERT(m_lastPreviewParameters.ContainsKey(changedResource->GetGUID()));
    const ResourcePreviewFactoryParameters &lastParams =
        m_lastPreviewParameters.Get(changedResource->GetGUID());

    // Update preview
    FillTextureWithPreview(resPreviewTex, changedResT, lastParams);

    SetReceiveEvents(true);
}

template <class T>
Scene *ResourcePreviewFactory<T>::GetPreviewScene() const
{
    return m_previewScene;
}

template <class T>
Camera *ResourcePreviewFactory<T>::GetPreviewCamera() const
{
    return p_previewCamera;
}

template <class T>
GameObject *ResourcePreviewFactory<T>::GetPreviewGameObjectContainer() const
{
    return p_previewGameObjectContainer;
}
}
