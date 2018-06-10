#include "BangEditor/SelectionGizmosManager.h"

#include "Bang/Gizmos.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/ComponentsGizmos.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
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

    ComponentsGizmos *csg = GameObject::Create<ComponentsGizmos>();
    p_componentsGizmos = csg;
    GetComponentsGizmos()->SetReferencedGameObject(nullptr);

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
}

SelectionGizmosManager::~SelectionGizmosManager()
{
}

void SelectionGizmosManager::Update()
{
    GetTransformGizmo()->Start();
    GetComponentsGizmos()->Start();
    GetGameObjectSelectionGizmo()->Start();

    GetTransformGizmo()->Update();
    GetComponentsGizmos()->Update();
    GetGameObjectSelectionGizmo()->Update();
}

void SelectionGizmosManager::OnBeginRender(Scene *scene)
{
    GetGameObjectSelectionGizmo()->SetParent(scene);
    GetComponentsGizmos()->SetParent(scene);
    GetTransformGizmo()->SetParent(scene);
    GetTransformGizmo()->OnBeginRender(scene);
}

void SelectionGizmosManager::OnEndRender(Scene*)
{
    GetTransformGizmo()->SetParent(nullptr);
    GetTransformGizmo()->OnEndRender(nullptr);
    GetGameObjectSelectionGizmo()->SetParent(nullptr);
    GetComponentsGizmos()->SetParent(nullptr);
}

SelectionGizmosManager* SelectionGizmosManager::GetInstance()
{
    return EditSceneGameObjects::GetInstance()->GetSelectionGizmosManager();
}

void SelectionGizmosManager::OnGameObjectSelected(GameObject *selectedGameObject)
{
    GetTransformGizmo()->SetReferencedGameObject(selectedGameObject);
    GetComponentsGizmos()->SetReferencedGameObject(selectedGameObject);
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(selectedGameObject);

    GetTransformGizmo()->SetEnabled( selectedGameObject != nullptr );
    GetGameObjectSelectionGizmo()->SetEnabled( selectedGameObject != nullptr );
}

void SelectionGizmosManager::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
}

TransformGizmo* SelectionGizmosManager::GetTransformGizmo() const
{
    return p_transformGizmo;
}

ComponentsGizmos *SelectionGizmosManager::GetComponentsGizmos() const
{
    return p_componentsGizmos;
}

GameObjectSelectionGizmo *SelectionGizmosManager::GetGameObjectSelectionGizmo() const
{
    return p_gameObjectSelectionGizmo;
}

