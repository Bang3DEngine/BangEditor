#ifndef SIWPHYSICSSETTINGS_H
#define SIWPHYSICSSETTINGS_H

#include <vector>

#include "Bang/Array.tcc"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

namespace Bang
{
class IEventsValueChanged;
class UIInputNumber;
template <class>
class EventEmitter;
}

using namespace Bang;
namespace BangEditor
{
class UIInputVector;

class SIWPhysicsSettings : public SettingsInspectorWidget
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
}

#endif  // SIWPHYSICSSETTINGS_H
