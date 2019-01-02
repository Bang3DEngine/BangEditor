#ifndef SIWBUILDSETTINGS_H
#define SIWBUILDSETTINGS_H

#include "Bang/Bang.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/EventEmitter.tcc"
#include "Bang/EventListener.h"
#include "Bang/IEvents.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class UIInputFile;
class SIWBuildSettings : public SettingsInspectorWidget
{
public:
    SIWBuildSettings();
    virtual ~SIWBuildSettings() override;

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;

    // IEventsValueChanged
    virtual void OnValueChanged(EventEmitter<IEventsValueChanged> *ee) override;

private:
    UIInputFile *p_initialSceneInput = nullptr;
};
}

#endif  // SIWBUILDSETTINGS_H
