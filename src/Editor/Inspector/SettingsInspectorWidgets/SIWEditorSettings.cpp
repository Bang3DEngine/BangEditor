#include "BangEditor/SIWEditorSettings.h"

using namespace Bang;
using namespace BangEditor;

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
