#include "BangEditor/ResourceInspectorWidgetFactory.h"

#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Path.h"
#include "BangEditor/RIWAnimation.h"
#include "BangEditor/RIWAnimatorStateMachine.h"
#include "BangEditor/RIWAudioClip.h"
#include "BangEditor/RIWBehaviour.h"
#include "BangEditor/RIWMaterial.h"
#include "BangEditor/RIWMesh.h"
#include "BangEditor/RIWModel.h"
#include "BangEditor/RIWPhysicsMaterial.h"
#include "BangEditor/RIWTexture.h"
#include "BangEditor/RIWTextureCubeMap.h"
#include "BangEditor/ResourceInspectorWidget.h"

using namespace Bang;
using namespace BangEditor;

InspectorWidget *ResourceInspectorWidgetFactory::Create(const Path &path)
{
    ResourceInspectorWidget *riw = nullptr;
    if (path.HasExtension(Extensions::GetMaterialExtension()))
    {
        riw = new RIWMaterial();
    }
    else if (path.HasExtension(Extensions::GetPhysicsMaterialExtension()))
    {
        riw = new RIWPhysicsMaterial();
    }
    else if (path.HasExtension(Extensions::GetAnimatorStateMachineExtension()))
    {
        riw = new RIWAnimatorStateMachine();
    }
    else if (path.HasExtension(Extensions::GetImageExtensions()))
    {
        riw = new RIWTexture();
    }
    else if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        riw = new RIWModel();
    }
    else if (path.HasExtension(Extensions::GetMeshExtension()))
    {
        riw = new RIWMesh();
    }
    else if (path.HasExtension(Extensions::GetAudioClipExtensions()))
    {
        riw = new RIWAudioClip();
    }
    else if (path.HasExtension(Extensions::GetAnimationExtension()))
    {
        riw = new RIWAnimation();
    }
    else if (path.HasExtension(Extensions::GetTextureCubeMapExtension()))
    {
        riw = new RIWTextureCubeMap();
    }
    else if (path.HasExtension(Extensions::GetBehaviourExtensions()))
    {
        riw = new RIWBehaviour();
    }

    if (riw)
    {
        riw->Init();
        riw->SetPath(path);
    }

    return riw;
}
