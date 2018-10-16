#ifndef GIZMOSMANAGER_H
#define GIZMOSMANAGER_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsDestroy.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/Editor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class GameObject;
FORWARD   class IEventsDestroy;
FORWARD   class Scene;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ComponentsGizmos;
FORWARD class GameObjectSelectionGizmo;
FORWARD class IEventsEditor;
FORWARD class TransformGizmo;

class GizmosManager : public EventListener<IEventsEditor>,
                      public EventListener<IEventsDestroy>
{
public:
    GizmosManager();
    virtual ~GizmosManager() override;

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

