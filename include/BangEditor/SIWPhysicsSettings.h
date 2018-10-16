#ifndef SIWPHYSICSSETTINGS_H
#define SIWPHYSICSSETTINGS_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "Bang/IEventsValueChanged.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD   class IEventsValueChanged;
FORWARD   class UIInputNumber;
FORWARD_T class EventEmitter;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UIInputVector;

class SIWPhysicsSettings : public SettingsInspectorWidget,
                           public EventListener<IEventsValueChanged>
{
public:
	SIWPhysicsSettings();
	virtual ~SIWPhysicsSettings() override;

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

private:
    UIInputNumber *p_maxSubStepsInput = nullptr;
    UIInputNumber *p_stepSleepTimeInput = nullptr;
    UIInputVector *p_gravityInput = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // SIWPHYSICSSETTINGS_H

