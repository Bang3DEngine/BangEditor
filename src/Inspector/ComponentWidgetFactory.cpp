#include "BangEditor/ComponentWidgetFactory.h"

#include "Bang/Component.h"

#include "BangEditor/CWTransform.h"
#include "BangEditor/CWMeshRenderer.h"
#include "BangEditor/ComponentWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentWidget *ComponentWidgetFactory::Create(Component *component)
{
    ComponentWidget *cw = nullptr;
    String cName = component->GetClassName();
    if (cName == "Transform")
    {
        cw = GameObject::Create<CWTransform>();
    }
    else if (cName == "MeshRenderer")
    {
        cw = GameObject::Create<CWMeshRenderer>();
    }

    if (cw)
    {
        cw->SetComponent(component);
    }
    return cw;
}
