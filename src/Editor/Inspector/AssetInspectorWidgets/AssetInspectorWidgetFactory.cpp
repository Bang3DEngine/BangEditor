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
    AssetInspectorWidget *aiw = nullptr;
    if (path.HasExtension(Extensions::GetMaterialExtension()))
    {
        aiw = new AIWMaterial();
    }
    else if (path.HasExtension(Extensions::GetPhysicsMaterialExtension()))
    {
        aiw = new AIWPhysicsMaterial();
    }
    else if (path.HasExtension(Extensions::GetAnimatorStateMachineExtension()))
    {
        aiw = new AIWAnimatorStateMachine();
    }
    else if (path.HasExtension(Extensions::GetImageExtensions()))
    {
        aiw = new AIWTexture();
    }
    else if (path.HasExtension(Extensions::GetAnimatorLayerMaskExtension()))
    {
        aiw = new AIWAnimatorLayerMask();
    }
    else if (path.HasExtension(Extensions::GetModelExtensions()))
    {
        aiw = new AIWModel();
    }
    else if (path.HasExtension(Extensions::GetMeshExtension()))
    {
        aiw = new AIWMesh();
    }
    else if (path.HasExtension(Extensions::GetAudioClipExtensions()))
    {
        aiw = new AIWAudioClip();
    }
    else if (path.HasExtension(Extensions::GetAnimationExtension()))
    {
        aiw = new AIWAnimation();
    }
    else if (path.HasExtension(Extensions::GetTextureCubeMapExtension()))
    {
        aiw = new AIWTextureCubeMap();
    }
    else if (path.HasExtension(Extensions::GetBehaviourExtensions()))
    {
        aiw = new AIWBehaviour();
    }
    else
    {
        aiw = new AssetInspectorWidget();
    }

    if (aiw)
    {
        aiw->Init();
        aiw->SetPath(path);
    }

    return aiw;
}
