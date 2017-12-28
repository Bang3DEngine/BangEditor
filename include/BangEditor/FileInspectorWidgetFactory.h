#ifndef FILEINSPECTORWIDGETFACTORY_H
#define FILEINSPECTORWIDGETFACTORY_H

#include "Bang/Bang.h"
#include "BangEditor/BangEditor.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class InspectorWidget;

class FileInspectorWidgetFactory
{
public:
    static InspectorWidget* Create(const Path &path);

    FileInspectorWidgetFactory() = delete;
    virtual ~FileInspectorWidgetFactory() = delete;
};

NAMESPACE_BANG_EDITOR_END

#endif // FILEINSPECTORWIDGETFACTORY_H

