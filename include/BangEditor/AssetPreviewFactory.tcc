#pragma once

#include "Bang/AssetHandle.h"
#include "Bang/Assets.h"
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
#include "Bang/Scene.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"
#include "Bang/Sphere.h"
#include "Bang/TextureFactory.h"
#include "Bang/Transform.h"

#include "BangEditor/AssetPreviewFactory.h"

using namespace Bang;
namespace BangEditor
{
template <class T>
AssetPreviewFactory<T>::AssetPreviewFactory()
{
}

template <class T>
AssetPreviewFactory<T>::~AssetPreviewFactory()
{
    delete m_auxiliarFBToCopyTextures;

    if (m_previewScene)
    {
        GameObject::DestroyImmediate(m_previewScene);
    }

    for (AH<T> &asset : m_previewsAssets)
    {
        asset.Get()->EventEmitter<IEventsAsset>::UnRegisterListener(this);
    }
}

template <class T>
void AssetPreviewFactory<T>::Init()
{
    m_auxiliarFBToCopyTextures = new Framebuffer();
    m_auxiliarFBToCopyTextures->CreateAttachmentTex2D(GL::Attachment::COLOR0,
                                                      GL::ColorFormat::RGBA8);
}

template <class T>
void AssetPreviewFactory<T>::CreatePreviewScene()
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
    dLight->SetCastShadows(false);
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
AH<Texture2D> AssetPreviewFactory<T>::GetPreviewTextureFor_(
    T *asset,
    const AssetPreviewFactoryParameters &params)
{
    if (!asset)
    {
        return AH<Texture2D>(TextureFactory::GetWhiteTexture());
    }

    if (!m_lastPreviewParameters.ContainsKey(asset->GetGUID()) ||
        params != m_lastPreviewParameters.Get(asset->GetGUID()))
    {
        m_previewsMap.Remove(asset->GetGUID());
    }

    if (!m_previewsMap.ContainsKey(asset->GetGUID()))
    {
        // Create empty preview texture
        AH<Texture2D> previewTexAH = Assets::Create<Texture2D>();
        Texture2D *previewTex = previewTexAH.Get();

        // Fill texture with preview
        FillTextureWithPreview(previewTex, asset, params);

        // Add to preview map
        asset->EventEmitter<IEventsAsset>::RegisterListener(this);
        m_previewsMap.Add(asset->GetGUID(), previewTexAH);
        m_previewsAssets.PushBack(AH<T>(asset));
    }
    return m_previewsMap.Get(asset->GetGUID());
}

template <class T>
void AssetPreviewFactory<T>::FillTextureWithPreview(
    Texture2D *texture,
    T *asset,
    const AssetPreviewFactoryParameters &params)
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

    m_lastPreviewParameters.Add(asset->GetGUID(), params);
    OnUpdateTextureBegin(GetPreviewScene(),
                         GetPreviewCamera(),
                         GetPreviewGameObjectContainer(),
                         asset,
                         params);

    // Set camera
    Transform *camTR = GetPreviewCamera()->GetGameObject()->GetTransform();
    Sphere goSphere = GetPreviewGameObjectContainer()->GetBoundingSphereWorld();
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

    GL::SetViewport(0, 0, previewTextureSize, previewTextureSize);
    GEngine::GetInstance()->Render(GetPreviewScene(), GetPreviewCamera());

    GL::Bind(GL::BindTarget::READ_FRAMEBUFFER,
             GetPreviewCamera()->GetGBuffer()->GetGLId());
    GL::Bind(GL::BindTarget::DRAW_FRAMEBUFFER,
             m_auxiliarFBToCopyTextures->GetGLId());

    GL::ReadBuffer(GBuffer::AttColor);
    GL::DrawBuffers({GL::Attachment::COLOR0});

    GL::BlitFramebuffer(GL::GetViewportRect(),
                        GL::GetViewportRect(),
                        GL::FilterMode::NEAREST,
                        GL::BufferBit::COLOR);

    texture->Bind();
    texture->SetFilterMode(GL::FilterMode::BILINEAR);
    // texture->GenerateMipMaps();
    // texture->SetFilterMode(GL::FilterMode::Trilinear_LL);
    texture->PropagateAssetChanged();

    OnUpdateTextureEnd(GetPreviewScene(),
                       GetPreviewCamera(),
                       GetPreviewGameObjectContainer(),
                       asset,
                       params);

    // Restore OpenGL state
    GL::Pop(GL::Pushable::FRAMEBUFFER_AND_READ_DRAW_ATTACHMENTS);
    GL::Pop(GL::BindTarget::TEXTURE_2D);
    GL::Pop(GL::Pushable::VIEWPORT);
}

template <class T>
void AssetPreviewFactory<T>::OnAssetChanged(Asset *changedAsset)
{
    ASSERT(m_previewsMap.ContainsKey(changedAsset->GetGUID()));

    SetReceiveEvents(false);

    T *changedAssetT = SCAST<T *>(changedAsset);
    Texture2D *assetPreviewTex =
        m_previewsMap.Get(changedAssetT->GetGUID()).Get();

    ASSERT(m_lastPreviewParameters.ContainsKey(changedAsset->GetGUID()));
    const AssetPreviewFactoryParameters &lastParams =
        m_lastPreviewParameters.Get(changedAsset->GetGUID());

    // Update preview
    FillTextureWithPreview(assetPreviewTex, changedAssetT, lastParams);

    SetReceiveEvents(true);
}

template <class T>
Scene *AssetPreviewFactory<T>::GetPreviewScene() const
{
    return m_previewScene;
}

template <class T>
Camera *AssetPreviewFactory<T>::GetPreviewCamera() const
{
    return p_previewCamera;
}

template <class T>
GameObject *AssetPreviewFactory<T>::GetPreviewGameObjectContainer() const
{
    return p_previewGameObjectContainer;
}
}  // namespace BangEditor
