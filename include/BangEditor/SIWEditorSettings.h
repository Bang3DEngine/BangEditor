#ifndef SIWEDITORSETTINGS_H
#define SIWEDITORSETTINGS_H

#include "Bang/Bang.h"

#include "BangEditor/SettingsInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class SIWEditorSettings : public SettingsInspectorWidget
{
public:
    SIWEditorSettings();
    virtual ~SIWEditorSettings();

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // SIWEDITORSETTINGS_H

