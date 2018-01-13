#include "BangEditor/ComponentInspectorWidgetFactory.h"

#include "Bang/Component.h"

#include "BangEditor/CIWCamera.h"
#include "BangEditor/CIWTransform.h"
#include "BangEditor/CIWPointLight.h"
#include "BangEditor/CIWAudioSource.h"
#include "BangEditor/CIWMeshRenderer.h"
#include "BangEditor/CIWAudioListener.h"
#include "BangEditor/CIWDirectionalLight.h"
#include "BangEditor/CIWBehaviourContainer.h"
#include "BangEditor/ComponentInspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentInspectorWidget *ComponentInspectorWidgetFactory::Create(Component *component)
{
    ComponentInspectorWidget *ciw = nullptr;
    String cName = component->GetClassName();
    if (cName == "Transform")
    {
        ciw = GameObject::Create<CIWTransform>();
    }
    else if (cName == "MeshRenderer")
    {
        ciw = GameObject::Create<CIWMeshRenderer>();
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
