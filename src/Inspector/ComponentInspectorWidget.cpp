#include "BangEditor/ComponentInspectorWidget.h"

#include "Bang/Component.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

ComponentInspectorWidget::ComponentInspectorWidget()
{
}

ComponentInspectorWidget::~ComponentInspectorWidget()
{
}

void ComponentInspectorWidget::SetComponent(Component *comp)
{
    SetTitle(comp->GetClassName());
}

