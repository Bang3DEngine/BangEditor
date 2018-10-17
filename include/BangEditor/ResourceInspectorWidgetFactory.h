#ifndef FILEINSPECTORWIDGETFACTORY_H
#define FILEINSPECTORWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

namespace Bang
{
class Path;
}

using namespace Bang;
namespace BangEditor
{
class InspectorWidget;

class ResourceInspectorWidgetFactory
{
public:
    static InspectorWidget *Create(const Path &path);

    ResourceInspectorWidgetFactory() = delete;
    virtual ~ResourceInspectorWidgetFactory() = delete;
};
}

#endif  // FILEINSPECTORWIDGETFACTORY_H
