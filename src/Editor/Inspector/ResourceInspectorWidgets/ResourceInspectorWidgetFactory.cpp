#include "BangEditor/ResourceInspectorWidgetFactory.h"

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Extensions.h"

#include "BangEditor/RIWMesh.h"
#include "BangEditor/RIWModel.h"
#include "BangEditor/RIWTexture.h"
#include "BangEditor/RIWMaterial.h"
#include "BangEditor/RIWAnimation.h"
#include "BangEditor/RIWAudioClip.h"
#include "BangEditor/RIWBehaviour.h"
#include "BangEditor/RIWTextureCubeMap.h"
#include "BangEditor/RIWPhysicsMaterial.h"
#include "BangEditor/ResourceInspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

InspectorWidget *ResourceInspectorWidgetFactory::Create(const Path &path)
{
    ResourceInspectorWidget *riw = nullptr;
    if (path.HasExtension(Extensions::GetMaterialExtension()))
    {
        riw = GameObject::Create<RIWMaterial>();
    }
    else if (path.HasExtension(Extensions::GetPhysicsMaterialExtension()))
    {
        riw = GameObject::Create<RIWPhysicsMaterial>();
    }
    else if (path.HasExtension(Extensions::GetImageExtensions()))
    {
        riw = GameObject::Create<RIWTexture>();
    }
    else if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        riw = GameObject::Create<RIWModel>();
    }
    else if (path.HasExtension(Extensions::GetMeshExtension()))
    {
        riw = GameObject::Create<RIWMesh>();
    }
    else if (path.HasExtension(Extensions::GetAudioClipExtensions()))
    {
        riw = GameObject::Create<RIWAudioClip>();
    }
    else if (path.HasExtension(Extensions::GetAnimationExtension()))
    {
        riw = GameObject::Create<RIWAnimation>();
    }
    else if (path.HasExtension(Extensions::GetTextureCubeMapExtension()))
    {
        riw = GameObject::Create<RIWTextureCubeMap>();
    }
    else if (path.HasExtension(Extensions::GetBehaviourExtensions()))
    {
        riw = GameObject::Create<RIWBehaviour>();
    }

    if (riw)
    {
        riw->Init();
        riw->SetPath(path);
    }

    return riw;
}