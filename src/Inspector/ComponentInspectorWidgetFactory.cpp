#include "BangEditor/ComponentInspectorWidgetFactory.h"

#include "Bang/Component.h"

#include "BangEditor/CIWTransform.h"
#include "BangEditor/CIWMeshRenderer.h"
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
    else
    {
        ciw = GameObject::Create<ComponentInspectorWidget>();
    }

    if (ciw)
    {
        ciw->SetComponent(component);
    }
    return ciw;
}
