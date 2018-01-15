#include "BangEditor/FileInspectorWidget.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

void FileInspectorWidget::InitInnerWidgets()
{
    InspectorWidget::InitInnerWidgets();
}

void FileInspectorWidget::Update()
{
    InspectorWidget::Update();
}

void FileInspectorWidget::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        UpdateFromFileWhenChanged();
    }
}

Path FileInspectorWidget::GetPath() const
{
    return m_path;
}
