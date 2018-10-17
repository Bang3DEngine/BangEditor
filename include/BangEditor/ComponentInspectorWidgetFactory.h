#ifndef COMPONENTINSPECTORWIDGETFACTORY_H
#define COMPONENTINSPECTORWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Component;
}

using namespace Bang;
namespace BangEditor
{
class ComponentInspectorWidget;

class ComponentInspectorWidgetFactory
{
public:
    static ComponentInspectorWidget *Create(Component *component);

    ComponentInspectorWidgetFactory() = delete;
    virtual ~ComponentInspectorWidgetFactory() = delete;
};
}

#endif  // COMPONENTINSPECTORWIDGETFACTORY_H
