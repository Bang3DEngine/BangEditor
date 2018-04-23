#include "BangEditor/SelectionGizmosManager.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/ComponentsSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SelectionGizmosManager::SelectionGizmosManager()
{
    TransformGizmo *tg = GameObject::Create<TransformGizmo>();
    p_transformGizmo = tg;
    GetTransformGizmo()->SetReferencedGameObject(nullptr);

    ComponentsSelectionGizmo *csg = GameObject::Create<ComponentsSelectionGizmo>();
    p_componentsSelectionGizmo = csg;
    GetComponentsSelectionGizmo()->SetReferencedGameObject(nullptr);

    Editor::GetInstance()->EventEmitter<IEditorListener>::RegisterListener(this);
}

SelectionGizmosManager::~SelectionGizmosManager()
{
}

void SelectionGizmosManager::Update()
{
    GetTransformGizmo()->Start();
    GetComponentsSelectionGizmo()->Start();

    GetTransformGizmo()->Update();
    GetComponentsSelectionGizmo()->Update();
}

void SelectionGizmosManager::OnBeginRender(Scene *scene)
{
    GetTransformGizmo()->SetParent(scene);
    GetTransformGizmo()->OnBeginRender(scene);
    GetComponentsSelectionGizmo()->SetParent(scene);
}

void SelectionGizmosManager::OnEndRender(Scene*)
{
    GetTransformGizmo()->SetParent(nullptr);
    GetTransformGizmo()->OnEndRender(nullptr);
    GetComponentsSelectionGizmo()->SetParent(nullptr);
}

SelectionGizmosManager* SelectionGizmosManager::GetInstance()
{
    return EditSceneGameObjects::GetInstance()->GetSelectionGizmosManager();
}

void SelectionGizmosManager::OnGameObjectSelected(GameObject *selectedGameObject)
{
    GetTransformGizmo()->SetReferencedGameObject(selectedGameObject);
    GetComponentsSelectionGizmo()->SetReferencedGameObject(selectedGameObject);

    GetTransformGizmo()->SetEnabled( selectedGameObject != nullptr );
    GetComponentsSelectionGizmo()->SetEnabled( selectedGameObject != nullptr );
}

void SelectionGizmosManager::OnDestroyed(EventEmitter<IDestroyListener> *object)
{
}

TransformGizmo* SelectionGizmosManager::GetTransformGizmo() const
{
    return p_transformGizmo;
}

ComponentsSelectionGizmo *SelectionGizmosManager::GetComponentsSelectionGizmo() const
{
    return p_componentsSelectionGizmo;
}

