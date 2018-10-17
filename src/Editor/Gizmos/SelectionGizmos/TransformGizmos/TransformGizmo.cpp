#include "BangEditor/TransformGizmo.h"

#include <array>

#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/GameObjectFactory.h"
#include "Bang/HideFlags.h"
#include "Bang/Scene.h"
#include "Bang/Transform.h"
#include "BangEditor/GizmosManager.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/RectTransformSelectionGizmo.h"
#include "BangEditor/RotateGizmo.h"
#include "BangEditor/ScaleGizmo.h"
#include "BangEditor/TranslateGizmo.h"
#include "BangEditor/UISceneToolbar.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/UndoRedoSerializableChange.h"

using namespace Bang;
using namespace BangEditor;

TransformGizmo::TransformGizmo()
{
    SetName("TransformGizmo");

    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DONT_SERIALIZE);
    GetHideFlags().SetOn(HideFlag::DONT_CLONE);

    p_worldGizmoContainer = GameObjectFactory::CreateGameObject(true);
    p_worldGizmoContainer->AddComponent<HideInHierarchy>();

    p_canvasGizmoContainer = GameObjectFactory::CreateUIGameObject(true);
    p_canvasGizmoContainer->AddComponent<HideInHierarchy>();
    GameObjectFactory::CreateUICanvasInto(p_canvasGizmoContainer);

    p_worldGizmoContainer->SetParent(this);
    p_canvasGizmoContainer->SetParent(this);

    p_translateGizmo = GameObject::Create<TranslateGizmo>();
    p_rotateGizmo = GameObject::Create<RotateGizmo>();
    p_scaleGizmo = GameObject::Create<ScaleGizmo>();
    p_rectTransformGizmo = GameObject::Create<RectTransformSelectionGizmo>();

    p_translateGizmo->SetParent(p_worldGizmoContainer);
    p_rotateGizmo->SetParent(p_worldGizmoContainer);
    p_scaleGizmo->SetParent(p_worldGizmoContainer);
    p_rectTransformGizmo->SetParent(p_canvasGizmoContainer);

    p_translateGizmo->SetEnabled(false);
    p_rotateGizmo->SetEnabled(false);
    p_scaleGizmo->SetEnabled(false);
    p_rectTransformGizmo->SetEnabled(false);
}

TransformGizmo::~TransformGizmo()
{
}

void TransformGizmo::Update()
{
    SelectionGizmo::Update();

    GameObject *refGo = GetReferencedGameObject();
    if(!refGo || !refGo->GetTransform())
    {
        return;
    }

    UISceneToolbar *toolbar = UISceneToolbar::GetActive();
    TransformGizmoMode transformGizmoMode = toolbar->GetTransformGizmoMode();

    switch(transformGizmoMode)
    {
        case TransformGizmoMode::RECT:
            GetTransform()->SetLocalPosition(Vector3::Zero);
            GetTransform()->SetLocalRotation(Quaternion::Identity);
            GetTransform()->SetLocalScale(Vector3::One);
            break;

        default:
            GetTransform()->SetPosition(refGo->GetTransform()->GetPosition());
            GetTransform()->SetRotation(refGo->GetTransform()->GetRotation());
            GetTransform()->SetScale(GetScaleFactor());
    }

    GameObject *gizmoToEnable = nullptr;
    switch(transformGizmoMode)
    {
        case TransformGizmoMode::TRANSLATE:
            gizmoToEnable = p_translateGizmo;
            break;

        case TransformGizmoMode::ROTATE: gizmoToEnable = p_rotateGizmo; break;

        case TransformGizmoMode::SCALE: gizmoToEnable = p_scaleGizmo; break;

        case TransformGizmoMode::RECT:
            gizmoToEnable = p_rectTransformGizmo;
            break;

        default: break;
    }
    std::array<GameObject *, 4> trGizmos = {p_translateGizmo, p_rotateGizmo,
                                            p_scaleGizmo, p_rectTransformGizmo};
    for(GameObject *trGizmo : trGizmos)
    {
        trGizmo->SetEnabled(trGizmo == gizmoToEnable);
    }
}

void TransformGizmo::Render(RenderPass rp, bool renderChildren)
{
    GBuffer *gb = GEngine::GetActiveGBuffer();
    gb->PushDepthStencilTexture();
    gb->SetOverlayDepthStencil();

    GameObject::Render(rp, renderChildren);

    gb->PopDepthStencilTexture();
}

void TransformGizmo::OnBeginRender(Scene *scene)
{
    // p_worldGizmoContainer->SetParent(this);
    p_canvasGizmoContainer->SetParent(scene);
}

void TransformGizmo::OnEndRender(Scene *)
{
    // p_worldGizmoContainer->SetParent(this);
    p_canvasGizmoContainer->SetParent(this);
}

void TransformGizmo::OnGrabBegin()
{
    m_transformUndoMetaBefore =
        GetReferencedGameObject()->GetTransform()->GetMeta();
}

void TransformGizmo::OnGrabEnd()
{
    Transform *transform = GetReferencedGameObject()->GetTransform();
    MetaNode newUndoMeta = transform->GetMeta();

    UndoRedoManager::PushAction(new UndoRedoSerializableChange(
        transform, m_transformUndoMetaBefore, newUndoMeta));
}

void TransformGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_translateGizmo->SetReferencedGameObject(referencedGameObject);
    p_rotateGizmo->SetReferencedGameObject(referencedGameObject);
    p_scaleGizmo->SetReferencedGameObject(referencedGameObject);
    p_rectTransformGizmo->SetReferencedGameObject(referencedGameObject);
}

float TransformGizmo::GetScaleFactor() const
{
    return 0.15f * SelectionGizmo::GetScaleFactor();
}

TransformGizmo *TransformGizmo::GetInstance()
{
    return GizmosManager::GetInstance()->GetTransformGizmo();
}
