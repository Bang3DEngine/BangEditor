#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang/GameObject.h"

#include "BangEditor/BangEditor.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UITextRenderer;
FORWARD class UILayoutElement;
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
    void SetLabelsWidth(int labelsWidth);
    void AddWidget(GameObject *widget, int height = 15);
    void AddWidget(const String &labelContent, GameObject *widget, int height = 15);
    void SetWidgetEnabled(GameObject *widget, bool enabled);

    int GetLabelsWidth() const;
    GameObject *GetWidgetsContainer() const;

private:
    int m_labelsWidth = -1;
    List<GameObject*> p_widgets;
    Map<GameObject*, UILabel*> m_widgetToLabel;
    Map<UILabel*, UILayoutElement*> m_labelToLabelLE;

    UITextRenderer *p_title = nullptr;
    GameObject *p_widgetsContainer = nullptr;

    void AddWidgetInternal(const String &labelContent,
                           GameObject *widget,
                           int height,
                           bool addLabel);
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTORWIDGET_H

