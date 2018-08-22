#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include "Bang/Scene.h"

#include "BangEditor/SettingsInspectorWidget.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class UITabContainer;

class SIWRenderSettings : public SettingsInspectorWidget
{
public:
	SIWRenderSettings();
	virtual ~SIWRenderSettings();

    // SettingsInspectorWidget
    virtual void InitInnerWidgets() override;
    virtual void UpdateFromReference() override;
};

NAMESPACE_BANG_EDITOR_END

#endif // PROJECTSETTINGSDIALOG_H

