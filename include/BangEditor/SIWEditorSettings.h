#ifndef SIWEDITORSETTINGS_H
#define SIWEDITORSETTINGS_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class SIWEditorSettings : public SettingsInspectorWidget
{
public:
    SIWEditorSettings();
    virtual ~SIWEditorSettings() override;

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};
}

#endif  // SIWEDITORSETTINGS_H
