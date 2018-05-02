#include "BangEditor/SelectionGizmosManager.h"

#include "Bang/Gizmos.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/ComponentsSelectionGizmo.h"
#include "BangEditor/GameObjectSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SelectionGizmosManager::SelectionGizmosManager()
{
    TransformGizmo *tg = GameObject::Create<TransformGizmo>();
    p_transformGizmo = tg;
    GetTransformGizmo()->SetReferencedGameObject(nullptr);

    GameObjectSelectionGizmo *gsg = GameObject::Create<GameObjectSelectionGizmo>();
    p_gameObjectSelectionGizmo = gsg;
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(nullptr);

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
    GetGameObjectSelectionGizmo()->Start();

    GetTransformGizmo()->Update();
    GetComponentsSelectionGizmo()->Update();
    GetGameObjectSelectionGizmo()->Update();
}

void SelectionGizmosManager::OnBeginRender(Scene *scene)
{
    GetGameObjectSelectionGizmo()->SetParent(scene);
    GetComponentsSelectionGizmo()->SetParent(scene);
    GetTransformGizmo()->SetParent(scene);
    GetTransformGizmo()->OnBeginRender(scene);
}

void SelectionGizmosManager::OnEndRender(Scene*)
{
    GetTransformGizmo()->SetParent(nullptr);
    GetTransformGizmo()->OnEndRender(nullptr);
    GetGameObjectSelectionGizmo()->SetParent(nullptr);
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
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(selectedGameObject);

    GetTransformGizmo()->SetEnabled( selectedGameObject != nullptr );
    GetComponentsSelectionGizmo()->SetEnabled( selectedGameObject != nullptr );
    GetGameObjectSelectionGizmo()->SetEnabled( selectedGameObject != nullptr );
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

GameObjectSelectionGizmo *SelectionGizmosManager::GetGameObjectSelectionGizmo() const
{
    return p_gameObjectSelectionGizmo;
}

