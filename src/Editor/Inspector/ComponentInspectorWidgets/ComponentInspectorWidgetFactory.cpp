#include "BangEditor/ComponentInspectorWidgetFactory.h"

#include "Bang/Behaviour.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/GameObject.tcc"
#include "Bang/Serializable.h"
#include "Bang/String.h"
#include "BangEditor/CIWAnimator.h"
#include "BangEditor/CIWBehaviour.h"
#include "BangEditor/CIWBehaviourContainer.h"
#include "BangEditor/CIWCamera.h"
#include "BangEditor/CIWParticleSystem.h"
#include "BangEditor/CIWTransform.h"
#include "BangEditor/CIWUIImageRenderer.h"
#include "BangEditor/CIWUITextRenderer.h"
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
    else if (cName == "UITextRenderer")
    {
        ciw = new CIWUITextRenderer();
    }
    else if (cName == "UIImageRenderer")
    {
        ciw = new CIWUIImageRenderer();
    }
    else if (cName == "ParticleSystem")
    {
        ciw = new CIWParticleSystem();
    }
    else if (cName == "BehaviourContainer")
    {
        ciw = new CIWBehaviourContainer();
    }
    else if (cName == "Camera")
    {
        ciw = new CIWCamera();
    }
    else if (cName == "Animator")
    {
        ciw = new CIWAnimator();
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
