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

namespace Bang
{
class GameObject;
class IEventsDestroy;
class Scene;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class ComponentsGizmos;
class GameObjectSelectionGizmo;
class IEventsEditor;
class TransformGizmo;

class GizmosManager : public EventListener<IEventsEditor>,
                      public EventListener<IEventsDestroy>
{
public:
    GizmosManager();
    virtual ~GizmosManager() override;

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    Array<GameObject *> GetGameObjectsForSelection(uint priority) const;

    TransformGizmo *GetTransformGizmo() const;
    ComponentsGizmos *GetComponentsGizmos() const;
    GameObjectSelectionGizmo *GetGameObjectSelectionGizmo() const;

    static GizmosManager *GetInstance();

private:
    TransformGizmo *p_transformGizmo = nullptr;
    ComponentsGizmos *p_componentsGizmos = nullptr;
    GameObjectSelectionGizmo *p_gameObjectSelectionGizmo = nullptr;

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

    // IEventsDestroy
    void OnDestroyed(EventEmitter<IEventsDestroy> *object) override;
};
}

#endif  // GIZMOSMANAGER_H
