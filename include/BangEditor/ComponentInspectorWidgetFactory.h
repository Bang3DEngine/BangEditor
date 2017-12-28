#ifndef COMPONENTINSPECTORWIDGETFACTORY_H
#define COMPONENTINSPECTORWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Component;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ComponentInspectorWidget;

class ComponentInspectorWidgetFactory
{
public:
    static ComponentInspectorWidget* Create(Component *component);

    ComponentInspectorWidgetFactory() = delete;
    virtual ~ComponentInspectorWidgetFactory() = delete;
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTINSPECTORWIDGETFACTORY_H

