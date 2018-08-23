#include "BangEditor/FileInspectorWidgetFactory.h"

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Extensions.h"

#include "BangEditor/FIWModel.h"
#include "BangEditor/FIWTexture.h"
#include "BangEditor/FIWMaterial.h"
#include "BangEditor/FIWBehaviour.h"
#include "BangEditor/FIWTextureCubeMap.h"
#include "BangEditor/FIWPhysicsMaterial.h"
#include "BangEditor/FileInspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget *FileInspectorWidgetFactory::Create(const Path &path)
{
    FileInspectorWidget *fiw = nullptr;
    if (path.HasExtension(Extensions::GetMaterialExtension()))
    {
        fiw = GameObject::Create<FIWMaterial>();
    }
    if (path.HasExtension(Extensions::GetPhysicsMaterialExtension()))
    {
        fiw = GameObject::Create<FIWPhysicsMaterial>();
    }
    else if (path.HasExtension(Extensions::GetImageExtensions()))
    {
        fiw = GameObject::Create<FIWTexture>();
    }
    else if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        fiw = GameObject::Create<FIWModel>();
    }
    else if (path.HasExtension(Extensions::GetTextureCubeMapExtension()))
    {
        fiw = GameObject::Create<FIWTextureCubeMap>();
    }
    else if (path.HasExtension(Extensions::GetBehaviourExtensions()))
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
