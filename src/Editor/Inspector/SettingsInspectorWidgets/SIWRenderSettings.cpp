#include "BangEditor/SIWRenderSettings.h"

using namespace Bang;
using namespace BangEditor;

SIWRenderSettings::SIWRenderSettings()
{
}

SIWRenderSettings::~SIWRenderSettings()
{
}

void SIWRenderSettings::InitInnerWidgets()
{
    SettingsInspectorWidget::InitInnerWidgets();

    SetTitle("Render Settings");
    SetName("SIWRenderSettings");
}

void SIWRenderSettings::UpdateFromReference()
{
    SettingsInspectorWidget::UpdateFromReference();
}
