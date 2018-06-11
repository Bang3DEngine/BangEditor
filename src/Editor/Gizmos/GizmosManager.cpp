#include "BangEditor/GizmosManager.h"

#include "Bang/Gizmos.h"

#include "BangEditor/EditorScene.h"
#include "BangEditor/TransformGizmo.h"
#include "BangEditor/ComponentsGizmos.h"
#include "BangEditor/EditorSceneManager.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/GameObjectSelectionGizmo.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

GizmosManager::GizmosManager()
{
    TransformGizmo *tg = GameObject::Create<TransformGizmo>();
    p_transformGizmo = tg;
    GetTransformGizmo()->SetReferencedGameObject(nullptr);

    GameObjectSelectionGizmo *gsg = GameObject::Create<GameObjectSelectionGizmo>();
    p_gameObjectSelectionGizmo = gsg;
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(nullptr);

    ComponentsGizmos *csg = GameObject::Create<ComponentsGizmos>();
    p_componentsGizmos = csg;

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
}

GizmosManager::~GizmosManager()
{
}

void GizmosManager::Update()
{
    GetTransformGizmo()->Start();
    GetComponentsGizmos()->Start();
    GetGameObjectSelectionGizmo()->Start();

    GetTransformGizmo()->Update();
    GetComponentsGizmos()->Update();
    GetGameObjectSelectionGizmo()->Update();
}

void GizmosManager::OnBeginRender(Scene *scene)
{
    GetGameObjectSelectionGizmo()->SetParent(scene);
    GetComponentsGizmos()->SetParent(scene);
    GetTransformGizmo()->SetParent(scene);
    GetTransformGizmo()->OnBeginRender(scene);
}

void GizmosManager::OnEndRender(Scene*)
{
    GetTransformGizmo()->SetParent(nullptr);
    GetTransformGizmo()->OnEndRender(nullptr);
    GetGameObjectSelectionGizmo()->SetParent(nullptr);
    GetComponentsGizmos()->SetParent(nullptr);
}

GizmosManager* GizmosManager::GetInstance()
{
    return EditSceneGameObjects::GetInstance()->GetGizmosManager();
}

void GizmosManager::OnGameObjectSelected(GameObject *selectedGameObject)
{
    GetTransformGizmo()->SetReferencedGameObject(selectedGameObject);
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(selectedGameObject);

    GetTransformGizmo()->SetEnabled( selectedGameObject != nullptr );
    GetGameObjectSelectionGizmo()->SetEnabled( selectedGameObject != nullptr );
}

void GizmosManager::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
}

TransformGizmo* GizmosManager::GetTransformGizmo() const
{
    return p_transformGizmo;
}

ComponentsGizmos *GizmosManager::GetComponentsGizmos() const
{
    return p_componentsGizmos;
}

GameObjectSelectionGizmo *GizmosManager::GetGameObjectSelectionGizmo() const
{
    return p_gameObjectSelectionGizmo;
}

