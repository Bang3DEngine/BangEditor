#include "BangEditor/SIWPhysicsSettings.h"

USING_NAMESPACE_BANG
USING_NAMESPACE_BANG_EDITOR

SIWPhysicsSettings::SIWPhysicsSettings()
{
}

SIWPhysicsSettings::~SIWPhysicsSettings()
{
}

void SIWPhysicsSettings::InitInnerWidgets()
{
    SettingsInspectorWidget::InitInnerWidgets();

    SetTitle("Physics Settings");
    SetName("SIWPhysicsSettings");
}

void SIWPhysicsSettings::Update()
{
    SettingsInspectorWidget::Update();
}
