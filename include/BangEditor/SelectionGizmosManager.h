#ifndef SELECTIONGIZMOSMANAGER_H
#define SELECTIONGIZMOSMANAGER_H

#include "Bang/Bang.h"
#include "Bang/EventListener.h"
#include "Bang/IEventsDestroy.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TransformGizmo;
FORWARD class GameObjectSelectionGizmo;
FORWARD class ComponentsGizmos;

class SelectionGizmosManager : public EventListener<IEventsEditor>,
                               public EventListener<IEventsDestroy>
{
public:
    SelectionGizmosManager();
	virtual ~SelectionGizmosManager();

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    TransformGizmo *GetTransformGizmo() const;
    ComponentsGizmos *GetComponentsGizmos() const;
    GameObjectSelectionGizmo *GetGameObjectSelectionGizmo() const;

    static SelectionGizmosManager* GetInstance();

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

#endif // SELECTIONGIZMOSMANAGER_H

