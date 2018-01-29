#include "BangEditor/TransformGizmo.h"

#include "Bang/Input.h"
#include "Bang/Vector3.h"
#include "Bang/Transform.h"
#include "Bang/RectTransform.h"
#include "Bang/GameObjectFactory.h"

#include "BangEditor/ScaleGizmo.h"
#include "BangEditor/RotateGizmo.h"
#include "BangEditor/TranslateGizmo.h"
#include "BangEditor/HideInHierarchy.h"
#include "BangEditor/NotSelectableInEditor.h"
#include "BangEditor/RectTransformSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

TransformGizmo::TransformGizmo()
{
    SetName("TransformGizmo");

    AddComponent<Transform>();
    AddComponent<HideInHierarchy>();
    AddComponent<NotSelectableInEditor>();
    GetHideFlags().SetOn(HideFlag::DontSerialize);
    GetHideFlags().SetOn(HideFlag::DontClone);

    p_worldGizmoContainer = GameObjectFactory::CreateGameObject(true);
    p_canvasGizmoContainer = GameObjectFactory::CreateUIGameObject(true);
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

    GetTransform()->SetPosition( refGo->GetTransform()->GetPosition() );
    GetTransform()->SetRotation( refGo->GetTransform()->GetRotation() );

    GetTransform()->SetScale( GetScaleFactor() );

    if      (Input::GetKeyDown(Key::W)) { m_transformMode = TransformMode::Translate; }
    else if (Input::GetKeyDown(Key::E)) { m_transformMode = TransformMode::Rotate; }
    else if (Input::GetKeyDown(Key::R)) { m_transformMode = TransformMode::Scale; }
    else if (Input::GetKeyDown(Key::T)) { m_transformMode = TransformMode::Rect; }

    switch (m_transformMode)
    {
        case TransformMode::Translate:
            p_translateGizmo->SetEnabled(true);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::Rotate:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(true);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::Scale:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(true);
            p_rectTransformGizmo->SetEnabled(false);
            break;

        case TransformMode::Rect:
            p_translateGizmo->SetEnabled(false);
            p_rotateGizmo->SetEnabled(false);
            p_scaleGizmo->SetEnabled(false);
            p_rectTransformGizmo->SetEnabled(true);
            break;
    }
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
            m_transformMode = TransformMode::Rect;
        }
        else if (m_transformMode == TransformMode::Rect)
        {
            m_transformMode = TransformMode::Translate;
        }

        Update(); // To avoid a bit of flickering
    }
}

float TransformGizmo::GetScaleFactor() const
{
    return 0.15f * SelectionGizmo::GetScaleFactor();
}
