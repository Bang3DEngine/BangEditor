#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include "Bang/BangDefines.h"
#include "Bang/Scene.h"
#include "BangEditor/BangEditor.h"
#include "BangEditor/SettingsInspectorWidget.h"

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabContainer;

class SIWRenderSettings : public SettingsInspectorWidget
{
public:
	SIWRenderSettings();
	virtual ~SIWRenderSettings() override;

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // PROJECTSETTINGSDIALOG_H

