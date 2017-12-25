#ifndef COMPONENTWIDGETFACTORY_H
#define COMPONENTWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Component;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class ComponentWidget;

class ComponentWidgetFactory
{
public:
    static ComponentWidget* Create(Component *component);

    ComponentWidgetFactory() = delete;
    virtual ~ComponentWidgetFactory() = delete;
};

NAMESPACE_BANG_EDITOR_END

#endif // COMPONENTWIDGETFACTORY_H

