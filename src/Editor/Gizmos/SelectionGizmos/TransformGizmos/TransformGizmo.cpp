#include "BangEditor/TransformGizmo.h"

#include "Bang/GL.h"
#include "Bang/Input.h"
#include "Bang/Scene.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/Vector3.h"
#include "Bang/Transform.h"
#include "Bang/RectTransform.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/ScaleGizmo.h"
#include "BangEditor/RotateGizmo.h"
#include "BangEditor/GizmosManager.h"
#include "BangEditor/TranslateGizmo.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/UndoRedoManager.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/UndoRedoSerializableChange.h"
#include "BangEditor/RectTransformSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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

    p_translateGizmo     = GameObject::Create<TranslateGizmo>();
    p_rotateGizmo        = GameObject::Create<RotateGizmo>();
    p_scaleGizmo         = GameObject::Create<ScaleGizmo>();
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
    if (!refGo || !refGo->GetTransform()) { return; }

    if      (Input::GetKeyDown(Key::W)) { m_transformMode = TransformMode::TRANSLATE; }
    else if (Input::GetKeyDown(Key::E)) { m_transformMode = TransformMode::ROTATE; }
    else if (Input::GetKeyDown(Key::R)) { m_transformMode = TransformMode::SCALE; }
    else if (Input::GetKeyDown(Key::T)) { m_transformMode = TransformMode::RECT; }

    switch (m_transformMode)
    {
        case TransformMode::RECT:
            GetTransform()->SetLocalPosition(Vector3::Zero);
            GetTransform()->SetLocalRotation(Quaternion::Identity);
            GetTransform()->SetLocalScale(Vector3::One);
        break;
        default:
            GetTransform()->SetPosition( refGo->GetTransform()->GetPosition() );
            GetTransform()->SetRotation( refGo->GetTransform()->GetRotation() );
            GetTransform()->SetScale( GetScaleFactor() );
    }

    switch (m_transformMode)
    {
        case TransformMode::TRANSLATE:
            p_translateGizmo->SetEnabled(true);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::ROTATE:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(true);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::SCALE:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(true);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::RECT:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(true);
            break;
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
    m_transformUndoXMLBefore = GetReferencedGameObject()->
                               GetTransform()->GetXMLInfo();
}

void TransformGizmo::OnGrabEnd()
{
    Transform *transform = GetReferencedGameObject()->GetTransform();
    XMLNode newUndoXML = transform->GetXMLInfo();

    UndoRedoManager::PushAction(
        new UndoRedoSerializableChange(transform, m_transformUndoXMLBefore,
                                       newUndoXML) );
}

void TransformGizmo::SetReferencedGameObject(GameObject *referencedGameObject)
{
    SelectionGizmo::SetReferencedGameObject(referencedGameObject);

    p_translateGizmo->SetReferencedGameObject(referencedGameObject);
    p_rotateGizmo->SetReferencedGameObject(referencedGameObject);
    p_scaleGizmo->SetReferencedGameObject(referencedGameObject);
    p_rectTransformGizmo->SetReferencedGameObject(referencedGameObject);

    // Change transform mode depending on if it has rect transform or not
    if (referencedGameObject)
    {
        if (referencedGameObject->HasComponent<RectTransform>())
        {
            m_transformMode = TransformMode::RECT;
        }
        else if (m_transformMode == TransformMode::RECT)
        {
            m_transformMode = TransformMode::TRANSLATE;
        }
        Update(); // To avoid a bit of flickering
    }
}

void TransformGizmo::SetTransformMode(TransformGizmo::TransformMode transformMode)
{
    m_transformMode = transformMode;
}

float TransformGizmo::GetScaleFactor() const
{
    return 0.15f * SelectionGizmo::GetScaleFactor();
}

TransformGizmo::TransformMode TransformGizmo::GetTransformMode() const
{
    return m_transformMode;
}

TransformGizmo *TransformGizmo::GetInstance()
{
    return GizmosManager::GetInstance()->GetTransformGizmo();
}