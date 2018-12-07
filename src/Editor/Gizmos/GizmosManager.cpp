#include "BangEditor/GizmosManager.h"

#include "Bang/EventEmitter.h"
#include "Bang/EventListener.tcc"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/IEventsDestroy.h"
#include "Bang/Scene.h"
#include "BangEditor/ComponentsGizmos.h"
#include "BangEditor/EditSceneGameObjects.h"
#include "BangEditor/Editor.h"
#include "BangEditor/GameObjectSelectionGizmo.h"
#include "BangEditor/IEventsEditor.h"
#include "BangEditor/TransformGizmo.h"

using namespace Bang;
using namespace BangEditor;

GizmosManager::GizmosManager()
{
    TransformGizmo *tg = new TransformGizmo();
    p_transformGizmo = tg;
    GetTransformGizmo()->SetReferencedGameObject(nullptr);

    GameObjectSelectionGizmo *gsg = new GameObjectSelectionGizmo();
    p_gameObjectSelectionGizmo = gsg;
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(nullptr);

    ComponentsGizmos *csg = new ComponentsGizmos();
    p_componentsGizmos = csg;

    Editor::GetInstance()->EventEmitter<IEventsEditor>::RegisterListener(this);
}

GizmosManager::~GizmosManager()
{
}

void GizmosManager::Update()
{
    GetTransformGizmo()->Start();
    GetGameObjectSelectionGizmo()->Start();
    GetComponentsGizmos()->Start();

    GetTransformGizmo()->Update();
    GetGameObjectSelectionGizmo()->Update();
    GetComponentsGizmos()->Update();
}

void GizmosManager::OnBeginRender(Scene *scene)
{
    GetGameObjectSelectionGizmo()->SetParent(scene);
    GetComponentsGizmos()->SetParent(scene);
    GetTransformGizmo()->SetParent(scene);
    GetTransformGizmo()->OnBeginRender(scene);
}

void GizmosManager::OnEndRender(Scene *)
{
    GetTransformGizmo()->SetParent(nullptr);
    GetTransformGizmo()->OnEndRender(nullptr);
    GetComponentsGizmos()->SetParent(nullptr);
    GetGameObjectSelectionGizmo()->SetParent(nullptr);
}

Array<GameObject *> GizmosManager::GetGameObjectsForSelection() const
{
    return {GetTransformGizmo(),
            GetComponentsGizmos(),
            GetGameObjectSelectionGizmo()};
}

GizmosManager *GizmosManager::GetInstance()
{
    return EditSceneGameObjects::GetInstance()->GetGizmosManager();
}

void GizmosManager::OnGameObjectSelected(GameObject *selectedGameObject)
{
    GetTransformGizmo()->SetReferencedGameObject(selectedGameObject);
    GetGameObjectSelectionGizmo()->SetReferencedGameObject(selectedGameObject);

    GetTransformGizmo()->SetEnabled(selectedGameObject != nullptr);
    GetGameObjectSelectionGizmo()->SetEnabled(selectedGameObject != nullptr);
}

void GizmosManager::OnDestroyed(EventEmitter<IEventsDestroy> *object)
{
}

TransformGizmo *GizmosManager::GetTransformGizmo() const
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
