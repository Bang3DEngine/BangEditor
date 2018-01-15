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
    static constexpr int DefaultLabelWidth  = 50;
    static constexpr int DefaultWidgetHeight = 15;

    InspectorWidget();
    virtual ~InspectorWidget();

    virtual void Init();
    virtual void InitInnerWidgets();

    virtual void SetTitle(const String &title);
    void SetLabelsWidth(int labelsWidth);

    virtual void Update() override;
    virtual void UpdateFromReference();

    void AddLabel(const String &content, int height = -1, int width = -1);
    void AddWidget(GameObject *widget,
                   int height = DefaultWidgetHeight);
    void AddWidget(const String &labelContent,
                   GameObject *widget,
                   int height = DefaultWidgetHeight);

    void SetWidgetEnabled(GameObject *widget, bool enabled);

    int GetLabelsWidth() const;
    GameObject *GetWidgetsContainer() const;

protected:
    virtual GameObject* CreateTitleGameObject();

private:
    int m_labelsWidth = -1;
    List<GameObject*> p_widgets;
    Map<GameObject*, UILabel*> m_widgetToLabel;
    Map<UILabel*, UILayoutElement*> m_labelToLabelLE;

    UITextRenderer *p_titleText = nullptr;
    UIImageRenderer *p_bgRenderer = nullptr;
    GameObject *p_widgetsContainer = nullptr;

    UILabel *CreateWidgetLabel(const String &content, int height, int width);

    void AddWidgetInternal(const String &labelContent,
                           GameObject *widget,
                           int height,
                           bool addLabel);
};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTORWIDGET_H

