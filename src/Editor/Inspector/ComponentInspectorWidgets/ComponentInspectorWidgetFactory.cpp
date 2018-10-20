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
#include "BangEditor/CIWCamera.h"
#include "BangEditor/CIWDirectionalLight.h"
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
        ciw = new CIWBehaviour();
    }
    else if (cName == "Transform")
    {
        ciw = new CIWTransform();
    }
    else if (cName == "RectTransform")
    {
        ciw = new CIWRectTransform();
    }
    else if (cName == "UITextRenderer")
    {
        ciw = new CIWUITextRenderer();
    }
    else if (cName == "UIImageRenderer")
    {
        ciw = new CIWUIImageRenderer();
    }
    else if (cName == "MeshRenderer")
    {
        ciw = new CIWMeshRenderer();
    }
    else if (cName == "RigidBody")
    {
        ciw = new CIWRigidBody();
    }
    else if (cName == "Rope")
    {
        ciw = new CIWRope();
    }
    else if (cName == "ParticleSystem")
    {
        ciw = new CIWParticleSystem();
    }
    else if (cName == "WaterRenderer")
    {
        ciw = new CIWWaterRenderer();
    }
    else if (cName == "SkinnedMeshRenderer")
    {
        ciw = new CIWSkinnedMeshRenderer();
    }
    else if (cName == "BehaviourContainer")
    {
        ciw = new CIWBehaviourContainer();
    }
    else if (cName == "DirectionalLight")
    {
        ciw = new CIWDirectionalLight();
    }
    else if (cName == "PointLight")
    {
        ciw = new CIWPointLight();
    }
    else if (cName == "Camera")
    {
        ciw = new CIWCamera();
    }
    else if (cName == "AudioListener")
    {
        ciw = new CIWAudioListener();
    }
    else if (cName == "AudioSource")
    {
        ciw = new CIWAudioSource();
    }
    else if (cName == "Animator")
    {
        ciw = new CIWAnimator();
    }
    else if (cName == "ReflectionProbe")
    {
        ciw = new CIWReflectionProbe();
    }
    else if (cName == "PostProcessEffect")
    {
        ciw = new CIWPostProcessEffect();
    }
    else if (cName == "PostProcessEffectSSAO")
    {
        ciw = new CIWPostProcessEffectSSAO();
    }
    else
    {
        ciw = new ComponentInspectorWidget();
    }

    if (ciw)
    {
        ciw->Init();
        ciw->SetComponent(component);
    }
    return ciw;
}
