#include "BangEditor/FileInspectorWidgetFactory.h"

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Extensions.h"

#include "BangEditor/FIWMaterial.h"
#include "BangEditor/FileInspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget *FileInspectorWidgetFactory::Create(const Path &path)
{
    FileInspectorWidget *fiw = nullptr;
    if (Extensions::Has(path, {Extensions::GetMaterialExtension()}))
    {
        fiw = GameObject::Create<FIWMaterial>();
    }

    if (fiw)
    {
        fiw->SetPath(path);
    }

    return fiw;
}
