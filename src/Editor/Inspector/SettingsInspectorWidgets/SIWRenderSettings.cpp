#include "BangEditor/SIWRenderSettings.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

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

void SIWRenderSettings::Update()
{
    SettingsInspectorWidget::Update();
}
