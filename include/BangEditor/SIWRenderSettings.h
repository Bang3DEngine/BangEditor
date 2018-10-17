#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include "Bang/BangDefines.h"
#include "Bang/Scene.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class UITabContainer;

class SIWRenderSettings : public SettingsInspectorWidget
{
public:
    SIWRenderSettings();
    virtual ~SIWRenderSettings() override;

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};
}

#endif  // PROJECTSETTINGSDIALOG_H
