#ifndef SIWPHYSICSSETTINGS_H
#define SIWPHYSICSSETTINGS_H

#include "Bang/Bang.h"

#include "BangEditor/SettingsInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SIWPhysicsSettings : public SettingsInspectorWidget
{
public:
	SIWPhysicsSettings();
	virtual ~SIWPhysicsSettings();

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SIWPHYSICSSETTINGS_H

