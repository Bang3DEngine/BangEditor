#ifndef SETTINGSINSPECTORWIDGET_H
#define SETTINGSINSPECTORWIDGET_H

#include "Bang/Bang.h"

#include "BangEditor/InspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SettingsInspectorWidget : public InspectorWidget
{
public:
    SettingsInspectorWidget();
    virtual ~SettingsInspectorWidget();

    // InspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void Update() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SETTINGSINSPECTORWIDGET_H

