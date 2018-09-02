#include "BangEditor/ResourceInspectorWidget.h"

#include "Bang/UICheckBox.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void ResourceInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
    GetInspectorWidgetTitle()->GetEnabledCheckBox()->
                               GetGameObject()->SetEnabled(false);
}

void ResourceInspectorWidget::Update()
{
    InspectorWidget::Update();
}

void ResourceInspectorWidget::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        UpdateFromFileWhenChanged();
    }
}

Path ResourceInspectorWidget::GetPath() const
{
    return m_path;
}
