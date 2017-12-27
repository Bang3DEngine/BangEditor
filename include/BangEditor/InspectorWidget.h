#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

class InspectorWidget : public GameObject
{
    GAMEOBJECT_EDITOR(InspectorWidget);

public:
    void SetBackgroundColor(const Color &bgColor);

protected:
    InspectorWidget();
    virtual ~InspectorWidget();

    void SetTitle(const String &title);
    void AddWidget(const String &labelContent,
                   GameObject *widget,
                   int height = 15);
    void SetWidgetEnabled(GameObject *widget, bool enabled);

    GameObject *GetLabelsContainer() const;
    GameObject *GetWidgetsContainer() const;

private:
    UITextRenderer *p_title = nullptr;
    GameObject *p_labelsContainer = nullptr;
    GameObject *p_widgetsContainer = nullptr;
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTORWIDGET_H

