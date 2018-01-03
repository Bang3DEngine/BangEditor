#ifndef COMPONENTINSPECTORWIDGET_H
#define COMPONENTINSPECTORWIDGET_H

#include "Bang/Bang.h"

#include "BangEditor/UIContextMenu.h"
#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class ComponentInspectorWidget : public InspectorWidget
{
    GAMEOBJECT_EDITOR(InspectorWidget);

protected:
    ComponentInspectorWidget();
    virtual ~ComponentInspectorWidget();

    virtual void SetComponent(Component *comp);

protected:
    Component *GetComponent() const;

private:
    Component *p_component = nullptr;
    UIContextMenu *p_contextMenu = nullptr;

    virtual void OnCreateContextMenu(MenuItem *menuRootItem);
    void MoveComponent(Component *comp, int offset);

    friend class ComponentInspectorWidgetFactory;
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTINSPECTORWIDGET_H

