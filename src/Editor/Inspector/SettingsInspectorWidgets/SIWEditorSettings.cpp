#include "BangEditor/SIWEditorSettings.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SIWEditorSettings::SIWEditorSettings()
{
}

SIWEditorSettings::~SIWEditorSettings()
{
}

void SIWEditorSettings::InitInnerWidgets()
{
    SettingsInspectorWidget::InitInnerWidgets();

    SetTitle("Editor Settings");
    SetName("SIWEditorSettings");
}

void SIWEditorSettings::UpdateFromReference()
{
    SettingsInspectorWidget::UpdateFromReference();
}

