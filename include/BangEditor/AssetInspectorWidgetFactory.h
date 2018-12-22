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

class AssetInspectorWidgetFactory
{
public:
    static InspectorWidget *Create(const Path &path);

    AssetInspectorWidgetFactory() = delete;
    virtual ~AssetInspectorWidgetFactory() = delete;
};
}

#endif  // FILEINSPECTORWIDGETFACTORY_H
