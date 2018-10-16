#ifndef FILEINSPECTORWIDGETFACTORY_H
#define FILEINSPECTORWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class Path;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class InspectorWidget;

class ResourceInspectorWidgetFactory
{
public:
    static InspectorWidget* Create(const Path &path);

    ResourceInspectorWidgetFactory() = delete;
    virtual ~ResourceInspectorWidgetFactory() = delete;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGETFACTORY_H

