#ifndef SELECTIONGIZMOSMANAGER_H
#define SELECTIONGIZMOSMANAGER_H

#include "Bang/Bang.h"

#include "BangEditor/Editor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class TransformGizmo;
FORWARD class ComponentsSelectionGizmo;

class SelectionGizmosManager : public IEditorListener
{
public:
    SelectionGizmosManager();
	virtual ~SelectionGizmosManager();

    void Update();

    void OnBeginRender(Scene *scene);
    void OnEndRender(Scene *scene);

    // IEditorListener
    void OnGameObjectSelected(GameObject *selectedGameObject) override;

private:
    TransformGizmo *p_transformGizmo = nullptr;
    ComponentsSelectionGizmo *p_componentsSelectionGizmo = nullptr;

    TransformGizmo *GetTransformGizmo() const;
    ComponentsSelectionGizmo *GetComponentsSelectionGizmo() const;
};

NAMESPACE_BANG_EDITOR_END

#endif // SELECTIONGIZMOSMANAGER_H

