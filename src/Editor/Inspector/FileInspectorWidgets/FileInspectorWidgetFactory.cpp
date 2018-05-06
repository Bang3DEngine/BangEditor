#include "BangEditor/FileInspectorWidgetFactory.h"

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Extensions.h"

#include "BangEditor/FIWTexture.h"
#include "BangEditor/FIWMaterial.h"
#include "BangEditor/FIWBehaviour.h"
#include "BangEditor/FIWTextureCubeMap.h"
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
    else if (Extensions::Has(path, {Extensions::GetImageExtensions()}))
    {
        fiw = GameObject::Create<FIWTexture>();
    }
    else if (Extensions::Has(path, {Extensions::GetTextureCubeMapExtension()}))
    {
        fiw = GameObject::Create<FIWTextureCubeMap>();
    }
    else if (Extensions::Has(path, {Extensions::GetBehaviourExtensions()}))
    {
        fiw = GameObject::Create<FIWBehaviour>();
    }

    if (fiw)
    {
        fiw->Init();
        fiw->SetPath(path);
    }

    return fiw;
}
