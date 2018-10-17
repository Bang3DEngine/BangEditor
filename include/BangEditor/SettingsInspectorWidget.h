#ifndef SETTINGSINSPECTORWIDGET_H
#define SETTINGSINSPECTORWIDGET_H

#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/InspectorWidget.h"

using namespace Bang;
namespace BangEditor
{
class SettingsInspectorWidget : public InspectorWidget
{
public:
    SettingsInspectorWidget();
    virtual ~SettingsInspectorWidget() override;

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};
}

#endif  // SETTINGSINSPECTORWIDGET_H
