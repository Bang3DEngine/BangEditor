#include "BangEditor/AssetInspectorWidgetFactory.h"

#include "Bang/Extensions.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Path.h"
#include "BangEditor/AIWAnimation.h"
#include "BangEditor/AIWAnimatorLayerMask.h"
#include "BangEditor/AIWAnimatorStateMachine.h"
#include "BangEditor/AIWAudioClip.h"
#include "BangEditor/AIWBehaviour.h"
#include "BangEditor/AIWMaterial.h"
#include "BangEditor/AIWMesh.h"
#include "BangEditor/AIWModel.h"
#include "BangEditor/AIWPhysicsMaterial.h"
#include "BangEditor/AIWTexture.h"
#include "BangEditor/AIWTextureCubeMap.h"
#include "BangEditor/AssetInspectorWidget.h"

using namespace Bang;
using namespace BangEditor;

InspectorWidget *AssetInspectorWidgetFactory::Create(const Path &path)
{
    AssetInspectorWidget *riw = nullptr;
    if (path.HasExtension(Extensions::GetMaterialExtension()))
    {
        riw = new AIWMaterial();
    }
    else if (path.HasExtension(Extensions::GetPhysicsMaterialExtension()))
    {
        riw = new AIWPhysicsMaterial();
    }
    else if (path.HasExtension(Extensions::GetAnimatorStateMachineExtension()))
    {
        riw = new AIWAnimatorStateMachine();
    }
    else if (path.HasExtension(Extensions::GetImageExtensions()))
    {
        riw = new AIWTexture();
    }
    else if (path.HasExtension(Extensions::GetAnimatorLayerMaskExtension()))
    {
        riw = new AIWAnimatorLayerMask();
    }
    else if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        riw = new AIWModel();
    }
    else if (path.HasExtension(Extensions::GetMeshExtension()))
    {
        riw = new AIWMesh();
    }
    else if (path.HasExtension(Extensions::GetAudioClipExtensions()))
    {
        riw = new AIWAudioClip();
    }
    else if (path.HasExtension(Extensions::GetAnimationExtension()))
    {
        riw = new AIWAnimation();
    }
    else if (path.HasExtension(Extensions::GetTextureCubeMapExtension()))
    {
        riw = new AIWTextureCubeMap();
    }
    else if (path.HasExtension(Extensions::GetBehaviourExtensions()))
    {
        riw = new AIWBehaviour();
    }

    if (riw)
    {
        riw->Init();
        riw->SetPath(path);
    }

    return riw;
}
