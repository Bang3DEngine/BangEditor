#ifndef GIZMOSMANAGER_H
#define GIZMOSMANAGER_H

#include "Bang/Bang.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TransformGizmo;
FORWARD class ComponentsGizmos;
FORWARD class GameObjectSelectionGizmo;

class GizmosManager : public EventListener<IEventsEditor>,
                               public EventListener<IEventsDestroy>
{
public:
    GizmosManager();
    virtual ~GizmosManager();

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    TransformGizmo *GetTransformGizmo() const;
    ComponentsGizmos *GetComponentsGizmos() const;
    GameObjectSelectionGizmo *GetGameObjectSelectionGizmo() const;

    static GizmosManager* GetInstance();

private:
    TransformGizmo *p_transformGizmo = nullptr;
    ComponentsGizmos *p_componentsGizmos = nullptr;
    GameObjectSelectionGizmo *p_gameObjectSelectionGizmo = nullptr;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};

NAMESPACE_BANG_EDITOR_END

#endif // GIZMOSMANAGER_H

