#include "BangEditor/ComponentInspectorWidgetFactory.h"

#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Serializable.h"
#include "Bang/String.h"
#include "BangEditor/CIWAnimator.h"
#include "BangEditor/CIWAudioListener.h"
#include "BangEditor/CIWAudioSource.h"
#include "BangEditor/CIWBehaviour.h"
#include "BangEditor/CIWBehaviourContainer.h"
#include "BangEditor/CIWBoxCollider.h"
#include "BangEditor/CIWCamera.h"
#include "BangEditor/CIWCapsuleCollider.h"
#include "BangEditor/CIWDirectionalLight.h"
#include "BangEditor/CIWMeshCollider.h"
#include "BangEditor/CIWMeshRenderer.h"
#include "BangEditor/CIWParticleSystem.h"
#include "BangEditor/CIWPointLight.h"
#include "BangEditor/CIWPostProcessEffect.h"
#include "BangEditor/CIWPostProcessEffectSSAO.h"
#include "BangEditor/CIWRectTransform.h"
#include "BangEditor/CIWReflectionProbe.h"
#include "BangEditor/CIWRigidBody.h"
#include "BangEditor/CIWRope.h"
#include "BangEditor/CIWSkinnedMeshRenderer.h"
#include "BangEditor/CIWSphereCollider.h"
#include "BangEditor/CIWTransform.h"
#include "BangEditor/CIWUIImageRenderer.h"
#include "BangEditor/CIWUITextRenderer.h"
#include "BangEditor/CIWWaterRenderer.h"
#include "BangEditor/ComponentInspectorWidget.h"

namespace Bang
{
class Behaviour;
}

using namespace Bang;
using namespace BangEditor;

ComponentInspectorWidget *ComponentInspectorWidgetFactory::Create(
    Component *component)
{
    ComponentInspectorWidget *ciw = nullptr;

    String cName = component->GetClassName();
    if (DCAST<Behaviour *>(component))
    {
        ciw = GameObject::Create<CIWBehaviour>();
    }
    else if (cName == "Transform")
    {
        ciw = GameObject::Create<CIWTransform>();
    }
    else if (cName == "RectTransform")
    {
        ciw = GameObject::Create<CIWRectTransform>();
    }
    else if (cName == "UITextRenderer")
    {
        ciw = GameObject::Create<CIWUITextRenderer>();
    }
    else if (cName == "UIImageRenderer")
    {
        ciw = GameObject::Create<CIWUIImageRenderer>();
    }
    else if (cName == "MeshRenderer")
    {
        ciw = GameObject::Create<CIWMeshRenderer>();
    }
    else if (cName == "RigidBody")
    {
        ciw = GameObject::Create<CIWRigidBody>();
    }
    else if (cName == "Rope")
    {
        ciw = GameObject::Create<CIWRope>();
    }
    else if (cName == "ParticleSystem")
    {
        ciw = GameObject::Create<CIWParticleSystem>();
    }
    else if (cName == "BoxCollider")
    {
        ciw = GameObject::Create<CIWBoxCollider>();
    }
    else if (cName == "MeshCollider")
    {
        ciw = GameObject::Create<CIWMeshCollider>();
    }
    else if (cName == "CapsuleCollider")
    {
        ciw = GameObject::Create<CIWCapsuleCollider>();
    }
    else if (cName == "SphereCollider")
    {
        ciw = GameObject::Create<CIWSphereCollider>();
    }
    else if (cName == "WaterRenderer")
    {
        ciw = GameObject::Create<CIWWaterRenderer>();
    }
    else if (cName == "SkinnedMeshRenderer")
    {
        ciw = GameObject::Create<CIWSkinnedMeshRenderer>();
    }
    else if (cName == "BehaviourContainer")
    {
        ciw = GameObject::Create<CIWBehaviourContainer>();
    }
    else if (cName == "DirectionalLight")
    {
        ciw = GameObject::Create<CIWDirectionalLight>();
    }
    else if (cName == "PointLight")
    {
        ciw = GameObject::Create<CIWPointLight>();
    }
    else if (cName == "Camera")
    {
        ciw = GameObject::Create<CIWCamera>();
    }
    else if (cName == "AudioListener")
    {
        ciw = GameObject::Create<CIWAudioListener>();
    }
    else if (cName == "AudioSource")
    {
        ciw = GameObject::Create<CIWAudioSource>();
    }
    else if (cName == "Animator")
    {
        ciw = GameObject::Create<CIWAnimator>();
    }
    else if (cName == "ReflectionProbe")
    {
        ciw = GameObject::Create<CIWReflectionProbe>();
    }
    else if (cName == "PostProcessEffect")
    {
        ciw = GameObject::Create<CIWPostProcessEffect>();
    }
    else if (cName == "PostProcessEffectSSAO")
    {
        ciw = GameObject::Create<CIWPostProcessEffectSSAO>();
    }
    else
    {
        ciw = GameObject::Create<ComponentInspectorWidget>();
    }

    if (ciw)
    {
        ciw->Init();
        ciw->SetComponent(component);
    }
    return ciw;
}
