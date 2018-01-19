#include "BangEditor/SelectionGizmosManager.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/ComponentsSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SelectionGizmosManager::SelectionGizmosManager()
{
    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
}

SelectionGizmosManager::~SelectionGizmosManager()
{
}

void SelectionGizmosManager::Update()
{
    if (GetCurrentTransformGizmo())
    {
        GetCurrentTransformGizmo()->Update();
    }

    if (GetCurrentComponentsSelectionGizmo())
    {
        GetCurrentComponentsSelectionGizmo()->Update();
    }
}

void SelectionGizmosManager::OnBeginRender(Scene *scene)
{
    if (GetCurrentTransformGizmo())
    {
        GetCurrentTransformGizmo()->SetParent(scene);
    }

    if (GetCurrentComponentsSelectionGizmo())
    {
        GetCurrentComponentsSelectionGizmo()->SetParent(scene);
    }
}

void SelectionGizmosManager::OnEndRender(Scene*)
{
    if (GetCurrentTransformGizmo())
    {
        GetCurrentTransformGizmo()->SetParent(nullptr);
    }

    if (GetCurrentComponentsSelectionGizmo())
    {
        GetCurrentComponentsSelectionGizmo()->SetParent(nullptr);
    }
}

void SelectionGizmosManager::OnGameObjectSelected(GameObject *selectedGameObject)
{
    // Destroy previous transform gizmo
    if (GetCurrentTransformGizmo())
    {
        GameObject::Destroy( GetCurrentTransformGizmo() );
        p_currentTransformGizmo = nullptr;
    }

    // Destroy previous components selection gizmo
    if (GetCurrentComponentsSelectionGizmo())
    {
        GameObject::Destroy( GetCurrentComponentsSelectionGizmo() );
        p_currentComponentsSelectionGizmo = nullptr;
    }

    if (selectedGameObject)
    {
        // Create new transform gizmo
        ASSERT(!GetCurrentTransformGizmo());
        TransformGizmo *tg = GameObject::Create<TransformGizmo>();
        p_currentTransformGizmo = tg;
        GetCurrentTransformGizmo()->SetReferencedGameObject(selectedGameObject);
        GetCurrentTransformGizmo()->
                EventEmitter<IDestroyListener>::RegisterListener(this);

        // Create new components selection gizmo
        ASSERT(!GetCurrentComponentsSelectionGizmo());
        ComponentsSelectionGizmo *csg = GameObject::Create<ComponentsSelectionGizmo>();
        p_currentComponentsSelectionGizmo = csg;
        GetCurrentComponentsSelectionGizmo()->SetReferencedGameObject(selectedGameObject);
        GetCurrentComponentsSelectionGizmo()->
                EventEmitter<IDestroyListener>::RegisterListener(this);
    }
}

void SelectionGizmosManager::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
    if (GetCurrentTransformGizmo() == object)
    {
        p_currentTransformGizmo = nullptr;
    }

    if (GetCurrentComponentsSelectionGizmo() == object)
    {
        p_currentComponentsSelectionGizmo = nullptr;
    }
}

TransformGizmo* SelectionGizmosManager::GetCurrentTransformGizmo() const
{
    return p_currentTransformGizmo;
}

ComponentsSelectionGizmo *SelectionGizmosManager::GetCurrentComponentsSelectionGizmo() const
{
    return p_currentComponentsSelectionGizmo;
}

