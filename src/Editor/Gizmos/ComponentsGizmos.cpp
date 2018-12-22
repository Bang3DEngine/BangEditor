#include "BangEditor/ComponentsGizmos.h"

#include "Bang/AABox.h"
#include "Bang/AudioSource.h"
#include "Bang/Axis.h"
#include "Bang/BoxCollider.h"
#include "Bang/Camera.h"
#include "Bang/CapsuleCollider.h"
#include "Bang/Color.h"
#include "Bang/Component.h"
#include "Bang/Debug.h"
#include "Bang/DecalRenderer.h"
#include "Bang/DirectionalLight.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GL.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/HideFlags.h"
#include "Bang/Material.h"
#include "Bang/Math.h"
#include "Bang/Mesh.h"
#include "Bang/MeshFactory.h"
#include "Bang/MeshRenderer.h"
#include "Bang/NavigationMesh.h"
#include "Bang/ParticleSystem.h"
#include "Bang/PointLight.h"
#include "Bang/Quaternion.h"
#include "Bang/ReflectionProbe.h"
#include "Bang/RenderFactory.h"
#include "Bang/Rope.h"
#include "Bang/Scene.h"
#include "Bang/SphereCollider.h"
#include "Bang/Transform.h"
#include "Bang/Vector3.h"
#include "BangEditor/Editor.h"
#include "BangEditor/EditorCamera.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditorTextureFactory.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/Selection.h"
#include "BangEditor/SelectionOptions.h"

using namespace Bang;
using namespace BangEditor;

ComponentsGizmos::ComponentsGizmos()
{
    SetName("ComponentsGizmos");

    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DONT_SERIALIZE);
    GetHideFlags().SetOn(HideFlag::DONT_CLONE);
}

ComponentsGizmos::~ComponentsGizmos()
{
}

void ComponentsGizmos::Render(RenderPass rp, bool renderChildren)
{
    GameObject::Render(rp, renderChildren);

    if (rp == RenderPass::OVERLAY)
    {
        for (GameObject *selPlane : p_selectionPlanes)
        {
            selPlane->SetEnabled(false);
        }
        m_usedSelectionPlanesInThisFrame = 0;

        if (GameObject *selectedGameObject = Editor::GetSelectedGameObject())
        {
            for (Component *comp : selectedGameObject->GetComponents())
            {
                if (comp && comp->IsActiveRecursively())
                {
                    RenderComponentGizmos(comp, true);
                }
            }
        }

        Scene *openScene = EditorSceneManager::GetOpenScene();
        if (openScene)
        {
            RenderComponentGizmosWhenNotSelected(openScene);
        }
    }
}

float ComponentsGizmos::GetCameraDistScale(const Vector3 &position) const
{
    float camDistScale = 1.0f;

    if (Camera *cam = Camera::GetActive())
    {
        Vector3 camPos = cam->GetGameObject()->GetTransform()->GetPosition();
        if (cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
        {
            camDistScale = Vector3::Distance(camPos, position);
        }
    }

    return camDistScale;
}

void ComponentsGizmos::RenderComponentGizmosWhenNotSelected(GameObject *go)
{
    for (Component *comp : go->GetComponents())
    {
        if (comp)
        {
            RenderComponentGizmos(comp, false);
        }
    }

    for (GameObject *child : go->GetChildren())
    {
        if (child)
        {
            RenderComponentGizmosWhenNotSelected(child);
        }
    }
}

void ComponentsGizmos::RenderComponentGizmos(Component *comp,
                                             bool isBeingSelected)
{
    if (Camera *cam = DCAST<Camera *>(comp))
    {
        RenderCameraGizmo(cam, isBeingSelected);
    }
    else if (DecalRenderer *decalRend = DCAST<DecalRenderer *>(comp))
    {
        RenderDecalRendererGizmo(decalRend, isBeingSelected);
    }
    else if (PointLight *pl = DCAST<PointLight *>(comp))
    {
        RenderPointLightGizmo(pl, isBeingSelected);
    }
    else if (BoxCollider *bc = DCAST<BoxCollider *>(comp))
    {
        RenderBoxColliderGizmo(bc, isBeingSelected);
    }
    else if (CapsuleCollider *cc = DCAST<CapsuleCollider *>(comp))
    {
        RenderCapsuleColliderGizmo(cc, isBeingSelected);
    }
    else if (SphereCollider *sc = DCAST<SphereCollider *>(comp))
    {
        RenderSphereColliderGizmo(sc, isBeingSelected);
    }
    else if (DirectionalLight *dl = DCAST<DirectionalLight *>(comp))
    {
        RenderDirectionalLightGizmo(dl, isBeingSelected);
    }
    else if (ReflectionProbe *rp = DCAST<ReflectionProbe *>(comp))
    {
        RenderReflectionProbeGizmo(rp, isBeingSelected);
    }
    else if (NavigationMesh *nv = DCAST<NavigationMesh *>(comp))
    {
        RenderNavigationMeshGizmo(nv, isBeingSelected);
    }
    else if (ParticleSystem *ps = DCAST<ParticleSystem *>(comp))
    {
        RenderParticleSystemGizmo(ps, isBeingSelected);
    }
    else if (Rope *rope = DCAST<Rope *>(comp))
    {
        RenderRopeGizmo(rope, isBeingSelected);
    }
    else if (AudioSource *as = DCAST<AudioSource *>(comp))
    {
        RenderAudioSourceGizmo(as, isBeingSelected);
    }
}

void ComponentsGizmos::RenderBoxColliderGizmo(BoxCollider *bc,
                                              bool isBeingSelected)
{
    if (isBeingSelected && bc->IsEnabledRecursively())
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Transform *tr = bc->GetGameObject()->GetTransform();
        RenderFactory::Parameters params;
        params.thickness = 2.0f;
        params.wireframe = true;
        params.color = Color::Green();
        params.rotation = tr->GetRotation();
        params.cullFace = GL::CullFaceExt::BACK;
        Vector3 centerDisplacement = params.rotation * bc->GetCenter();
        Vector3 c = tr->GetPosition() + centerDisplacement;
        Vector3 hs = tr->GetScale() * bc->GetExtents() + 0.01f;
        RenderFactory::RenderBox(AABox(c - hs, c + hs), params);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderCapsuleColliderGizmo(CapsuleCollider *cc,
                                                  bool isBeingSelected)
{
    if (isBeingSelected && cc->IsEnabledRecursively())
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Transform *tr = cc->GetGameObject()->GetTransform();
        RenderFactory::Parameters params;
        params.thickness = 2.0f;
        params.wireframe = true;
        params.color = Color::Green();
        params.cullFace = GL::CullFaceExt::BACK;

        params.position = tr->GetPosition();

        params.rotation = tr->GetRotation();
        switch (cc->GetAxis())
        {
            case Axis3D::X:
                params.rotation =
                    params.rotation *
                    Quaternion::AngleAxis(SCAST<float>(Math::Pi) * 0.5f,
                                          Vector3::Forward());
                break;

            case Axis3D::Z:
                params.rotation =
                    params.rotation *
                    Quaternion::AngleAxis(SCAST<float>(Math::Pi) * 0.5f,
                                          Vector3::Right());
                break;

            default: break;
        }

        Vector3 centerDisplacement = params.rotation * cc->GetCenter();
        params.position += centerDisplacement;

        float r = cc->GetScaledRadius() + 0.01f;
        float h = cc->GetScaledHeight() + 0.01f;
        RenderFactory::RenderWireframeCapsule(h, r, params);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderSphereColliderGizmo(SphereCollider *sc,
                                                 bool isBeingSelected)
{
    if (isBeingSelected && sc->IsEnabledRecursively())
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Transform *tr = sc->GetGameObject()->GetTransform();
        RenderFactory::Parameters params;
        params.thickness = 2.0f;
        params.wireframe = true;
        params.color = Color::Green();
        params.rotation = tr->GetRotation();
        params.cullFace = GL::CullFaceExt::BACK;
        Vector3 centerDisplacement = params.rotation * sc->GetCenter();
        Vector3 center = tr->GetPosition() + centerDisplacement;
        params.position = center;
        RenderFactory::RenderWireframeSphere(
            sc->GetScaledRadius() + 0.01f, false, params, 2, 2);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderCameraGizmo(Camera *cam, bool isBeingSelected)
{
    Transform *camTransform = cam->GetGameObject()->GetTransform();

    if (!isBeingSelected)
    {
        RenderFactory::Parameters params;
        params.position = camTransform->GetPosition();
        params.scale = Vector3(0.1f);
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(cam);
        AddSelectionPlaneFor(cam->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);

        // RH<Mesh> cameraMesh = MeshFactory::GetCamera();
        // Transform *camTransform = cam->GetGameObject()->GetTransform();
        // params.receivesLighting = true;
        // params.rotation = camTransform->GetRotation();
        // params.scale = Vector3(0.3f);
        // params.color = Color::White;
        //
        // GL::Disable(GL::Enablable::CULL_FACE);
        // GL::Disable(GL::Enablable::DEPTH_TEST);
        //
        // RenderFactory::RenderCustomMesh(cameraMesh.Get(), params);
        //
        // GL::Enable(GL::Enablable::DEPTH_TEST);
        // GL::Enable(GL::Enablable::CULL_FACE);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        RenderFactory::Parameters params;
        params.color = Color::Green();
        params.receivesLighting = false;

        if (cam->GetProjectionMode() == CameraProjectionMode::PERSPECTIVE)
        {
            RenderFactory::RenderFrustum(camTransform->GetForward(),
                                         camTransform->GetUp(),
                                         camTransform->GetPosition(),
                                         cam->GetZNear(),
                                         cam->GetZFar(),
                                         cam->GetFovDegrees(),
                                         GL::GetViewportAspectRatio(),
                                         params);
        }
        else
        {
            AABox orthoBox;
            Vector3 pos = camTransform->GetPosition();
            Vector2 orthoSize =
                Vector2(cam->GetOrthoWidth(), cam->GetOrthoHeight());
            orthoBox.SetMin(
                pos + Vector3(-orthoSize.x, -orthoSize.y, -cam->GetZNear()));
            orthoBox.SetMax(pos +
                            Vector3(orthoSize.x, orthoSize.y, -cam->GetZFar()));
            params.rotation = camTransform->GetRotation();
            RenderFactory::RenderWireframeBox(orthoBox, params);
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderDecalRendererGizmo(DecalRenderer *dr,
                                                bool isBeingSelected)
{
    Transform *tr = dr->GetGameObject()->GetTransform();

    Vector3 boxCenter = tr->GetPosition();

    RenderFactory::Parameters params;
    params.position = boxCenter;

    if (!isBeingSelected)
    {
        RenderFactory::Parameters paramsCpy = params;
        paramsCpy.scale = Vector3(0.1f);
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(dr);
        AddSelectionPlaneFor(dr->GetGameObject(), paramsCpy.scale, icon);
        RenderFactory::RenderIcon(icon, true, paramsCpy);
    }
    else
    {
        GL::Push(GL::Pushable::DEPTH_STATES);

        params.color = Color::Green();

        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        if (dr->GetIsPerspective())
        {
            Transform *tr = dr->GetGameObject()->GetTransform();
            params.position = Vector3::Zero();
            RenderFactory::RenderFrustum(tr->GetForward(),
                                         tr->GetUp(),
                                         tr->GetPosition(),
                                         dr->GetZNear(),
                                         dr->GetZFar(),
                                         dr->GetFieldOfViewDegrees(),
                                         dr->GetAspectRatio(),
                                         params);
        }
        else
        {
            Vector3 boxSize = dr->GetBoxSize();
            params.rotation = tr->GetRotation();
            params.scale = boxSize;

            AABox boxLocal = AABox(Vector3(-0.5f), Vector3(0.5f));
            RenderFactory::RenderWireframeBox(boxLocal, params);

            AABox boxLocalTrans = boxLocal;
            params.depthMask = false;
            params.color = params.color.WithAlpha(0.1f);
            boxLocalTrans.SetMin(boxLocalTrans.GetMin() + params.position);
            boxLocalTrans.SetMax(boxLocalTrans.GetMax() + params.position);
            RenderFactory::RenderBox(boxLocalTrans, params);

            Quaternion r = params.rotation;
            Vector3 boxMinWorld = params.position - r * boxSize * 0.5f;
            boxMinWorld -= (r * Vector3::Forward() * boxSize.z);

            params.position = Vector3::Zero();
            params.rotation = Quaternion::Identity();
            params.color = Color::Red();
            params.thickness = 5.0f;
            params.scale = Vector3::One();

            float markerSizeX = boxSize.x * 0.3f;
            RenderFactory::RenderLine(
                boxMinWorld,
                boxMinWorld + r * (Vector3::Right() * markerSizeX),
                params);

            float markerSizeY = boxSize.y * 0.3f;
            RenderFactory::RenderLine(
                boxMinWorld,
                boxMinWorld + r * (Vector3::Up() * markerSizeY),
                params);
        }

        gb->PopDepthStencilTexture();

        GL::Pop(GL::Pushable::DEPTH_STATES);
    }
}

void ComponentsGizmos::RenderPointLightGizmo(PointLight *pointLight,
                                             bool isBeingSelected)
{
    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.color = pointLight->GetColor().WithAlpha(1.0f);
    params.position =
        pointLight->GetGameObject()->GetTransform()->GetPosition();

    if (!isBeingSelected)
    {
        params.scale = Vector3(0.1f);
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(pointLight);
        AddSelectionPlaneFor(pointLight->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        params.thickness = 2.0f;
        RenderFactory::RenderWireframeSphere(
            pointLight->GetRange(), true, params, 1, 1, 32);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderDirectionalLightGizmo(DirectionalLight *dirLight,
                                                   bool isBeingSelected)
{
    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.color = dirLight->GetColor().WithAlpha(1.0f);

    if (!isBeingSelected)
    {
        params.position =
            dirLight->GetGameObject()->GetTransform()->GetPosition();
        params.scale = Vector3(0.1f);
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(dirLight);
        AddSelectionPlaneFor(dirLight->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        GameObject *lightGo = dirLight->GetGameObject();
        GameObject *camGo = Camera::GetActive()->GetGameObject();
        float distScale =
            Vector3::Distance(camGo->GetTransform()->GetPosition(),
                              lightGo->GetTransform()->GetPosition());
        const float radius = 0.03f * distScale;
        const float length = 0.2f * distScale;
        const Vector3 up = lightGo->GetTransform()->GetUp() * radius;
        const Vector3 right = lightGo->GetTransform()->GetRight() * radius;
        const Vector3 forward = lightGo->GetTransform()->GetForward() * length;
        const Vector3 center = lightGo->GetTransform()->GetPosition();

        params.thickness = 2.0f;
        for (float ang = 0.0f; ang <= 2 * SCAST<float>(Math::Pi);
             ang += SCAST<float>(Math::Pi) / 4.0f)
        {
            const Vector3 offx = right * Math::Cos(ang);
            const Vector3 offy = up * Math::Sin(ang);
            RenderFactory::RenderLine(
                center + offx + offy, center + offx + offy + forward, params);
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderReflectionProbeGizmo(ReflectionProbe *reflProbe,
                                                  bool isBeingSelected)
{
    Vector3 reflProbeCenter =
        reflProbe->GetGameObject()->GetTransform()->GetPosition();

    RenderFactory::Parameters params;
    params.color = Color::Green().WithValue(0.8f);
    params.scale = Vector3(0.15f);
    params.receivesLighting = false;
    params.position = reflProbeCenter;

    RenderFactory::RenderSphere(1.0f, params);

    if (isBeingSelected)
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Vector3 reflProbSize = reflProbe->GetSize();

        params.scale = Vector3::One();
        params.cullFace = GL::CullFaceExt::NONE;
        AABox reflProbeBox = AABox(reflProbeCenter + reflProbSize * 0.5f,
                                   reflProbeCenter - reflProbSize * 0.5f);

        if (reflProbe->GetIsBoxed())
        {
            params.thickness = 0.1f;
            params.wireframe = false;
            params.position = Vector3::Zero();
            params.color = params.color.WithAlpha(0.25f);
            RenderFactory::RenderBox(reflProbeBox, params);
        }

        params.wireframe = true;
        params.thickness = 3.0f;
        params.position = Vector3::Zero();
        params.color = params.color.WithAlpha(1.0f);
        RenderFactory::RenderWireframeBox(reflProbeBox, params);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderNavigationMeshGizmo(NavigationMesh *navigationMesh,
                                                 bool isBeingSelected)
{
    if (isBeingSelected)
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Vector2 gridSize = navigationMesh->GetGridSize();
        Vector3 gridCenter = navigationMesh->GetGridCenter();
        Vector2 cellSize = navigationMesh->GetCellSize();
        Vector3 gridLowerLeft = gridCenter - (gridSize.x0y() * 0.5f);

        RenderFactory::Parameters params;
        params.color = Color::Green();
        params.thickness = 2.0f;

        for (uint i = 0; i < navigationMesh->GetNumCells() + 1; ++i)
        {
            RenderFactory::RenderLine(
                gridLowerLeft + Vector3(i, 0, 0) * cellSize.x0y(),
                gridLowerLeft + Vector3(i, 0, 0) * cellSize.x0y() +
                    navigationMesh->GetGridSize().x0y() * Vector3(0, 0, 1),
                params);
            RenderFactory::RenderLine(
                gridLowerLeft + Vector3(0, 0, i) * cellSize.x0y(),
                gridLowerLeft + Vector3(0, 0, i) * cellSize.x0y() +
                    navigationMesh->GetGridSize().x0y() * Vector3(1, 0, 0),
                params);
        }

        params.color = Color::Red().WithAlpha(0.5f);
        for (uint i = 0; i < navigationMesh->GetNumCells(); ++i)
        {
            for (uint j = 0; j < navigationMesh->GetNumCells(); ++j)
            {
                if (navigationMesh->IsCellColliding(j, i))
                {
                    constexpr float boxHeight = 0.05f;
                    AABox cellBox;
                    cellBox.SetMin(navigationMesh->GetCellCenter(j, i) -
                                   navigationMesh->GetCellSize().x0y() * 0.5f -
                                   Vector3::Up() * boxHeight);
                    cellBox.SetMax(navigationMesh->GetCellCenter(j, i) +
                                   navigationMesh->GetCellSize().x0y() * 0.5f +
                                   Vector3::Up() * boxHeight);
                    RenderFactory::RenderBox(cellBox, params);
                }
            }
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderParticleSystemGizmo(ParticleSystem *particleSystem,
                                                 bool isBeingSelected)
{
    Transform *tr = particleSystem->GetGameObject()->GetTransform();
    Vector3 center = tr->GetPosition();

    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.scale = Vector3(0.1f);
    params.position = center;

    if (!isBeingSelected)
    {
        Texture2D *icon =
            EditorTextureFactory::GetComponentIcon(particleSystem);
        AddSelectionPlaneFor(
            particleSystem->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);
    }
    else
    {
        params.color = Color::Green();

        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        Quaternion rot = tr->GetRotation();

        switch (particleSystem->GetGenerationShape())
        {
            case ParticleGenerationShape::BOX:
            {
                AABox box;
                params.position = center;
                params.wireframe = true;
                params.rotation = rot;
                Vector3 boxSize = particleSystem->GetGenerationShapeBoxSize();
                box.SetMin(center - boxSize * 0.5f);
                box.SetMax(center + boxSize * 0.5f);
                RenderFactory::RenderBox(box, params);
            }
            break;

            case ParticleGenerationShape::CONE:
            {
                float hFovR =
                    particleSystem->GetGenerationShapeConeFOVRads() * 0.5f;
                params.position = center + rot * Vector3(0, 0, -1.0f);
                params.rotation = tr->GetRotation();
                params.scale = Vector3::One();
                float r = 2 * Math::Tan(hFovR);
                RenderFactory::RenderWireframeCircle(r, params, 32, false);

                params.position = Vector3::Zero();
                params.rotation = Quaternion::Identity();
                RenderFactory::RenderLine(
                    center, center + rot * Vector3(0, r, -1), params);
                RenderFactory::RenderLine(
                    center, center + rot * Vector3(r, 0, -1), params);
                RenderFactory::RenderLine(
                    center, center + rot * Vector3(-r, 0, -1), params);
                RenderFactory::RenderLine(
                    center, center + rot * Vector3(0, -r, -1), params);
            }
            break;
        }

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::RenderRopeGizmo(Rope *rope, bool isBeingSelected)
{
    Transform *tr = rope->GetGameObject()->GetTransform();
    Vector3 center = tr->GetPosition();

    RenderFactory::Parameters params;
    params.receivesLighting = false;
    params.scale = Vector3(0.1f);
    params.position = center;

    if (!isBeingSelected)
    {
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(rope);
        AddSelectionPlaneFor(rope->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);
    }
}

void ComponentsGizmos::RenderAudioSourceGizmo(AudioSource *audioSource,
                                              bool isBeingSelected)
{
    RenderFactory::Parameters params;
    params.color = Color::White();
    params.receivesLighting = false;
    params.position =
        audioSource->GetGameObject()->GetTransform()->GetPosition();

    if (!isBeingSelected)
    {
        params.scale = Vector3(0.1f);
        Texture2D *icon = EditorTextureFactory::GetComponentIcon(audioSource);
        AddSelectionPlaneFor(audioSource->GetGameObject(), params.scale, icon);
        RenderFactory::RenderIcon(icon, true, params);
    }
    else
    {
        GBuffer *gb = GEngine::GetActiveGBuffer();
        gb->PushDepthStencilTexture();
        gb->SetSceneDepthStencil();

        params.thickness = 2.0f;
        RenderFactory::RenderWireframeSphere(
            audioSource->GetRange(), true, params, 1, 2, 32);

        gb->PopDepthStencilTexture();
    }
}

void ComponentsGizmos::AddSelectionPlaneFor(GameObject *go,
                                            const Vector3 &scale,
                                            Texture2D *filterTexture)
{
    while (p_selectionPlanes.Size() <= m_usedSelectionPlanesInThisFrame)
    {
        GameObject *selPlane = GameObjectFactory::CreateGameObject();

        MeshRenderer *selPlaneRend = selPlane->AddComponent<MeshRenderer>();
        selPlaneRend->GetMaterial()->SetCullFace(GL::CullFaceExt::NONE);
        selPlaneRend->GetMaterial()->SetRenderPass(RenderPass::OVERLAY);
        selPlaneRend->SetMesh(MeshFactory::GetPlane().Get());
        selPlane->SetVisible(false);

        selPlane->AddComponent<SelectionOptions>();

        selPlane->SetParent(this);
        p_selectionPlanes.PushBack(selPlane);
    }

    GameObject *selPlane = p_selectionPlanes[m_usedSelectionPlanesInThisFrame];
    const Vector3 pos = go->GetTransform()->GetPosition();
    selPlane->GetTransform()->SetPosition(pos);
    if (SelectionOptions *selectionOptions =
            selPlane->GetComponent<SelectionOptions>())
    {
        selectionOptions->SetFilterTexture(filterTexture);
        selectionOptions->SetTargetGameObject(go);
    }

    Quaternion bbRotation;
    Vector3 bbScale;
    RenderFactory::GetBillboardTransform(pos, &bbRotation, &bbScale);
    selPlane->GetTransform()->SetRotation(bbRotation);
    selPlane->GetTransform()->SetScale(2.0f * scale * bbScale);

    selPlane->SetEnabled(true);

    ++m_usedSelectionPlanesInThisFrame;
}
