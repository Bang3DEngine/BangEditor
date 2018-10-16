#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include "Bang/BangDefines.h"
#include "Bang/GameObject.h"
#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/UMap.h"
#include "BangEditor/BangEditor.h"

namespace Bang {
class Color;
}  // namespace Bang

FORWARD NAMESPACE_BANG_BEGIN
FORWARD class UICheckBox;
FORWARD class UIImageRenderer;
FORWARD class UILabel;
FORWARD class UILayoutElement;
FORWARD class UITextRenderer;

FORWARD NAMESPACE_BANG_END

USING_NAMESPACE_BANG
NAMESPACE_BANG_EDITOR_BEGIN

FORWARD class InspectorWidgetTitle;

class InspectorWidget : public GameObject
{
    GAMEOBJECT_EDITOR(InspectorWidget);

public:
    virtual void Init();

    const String& GetTitle() const;
    void SetBackgroundColor(const Color &bgColor);

protected:
    static constexpr int DefaultLabelWidth  = 50;
    static constexpr int DefaultWidgetHeight = 20;

    InspectorWidget();
    virtual ~InspectorWidget();

    virtual void InitInnerWidgets();

    virtual void SetTitle(const String &title);
    void SetLabelsWidth(int labelsWidth);

    // GameObject
    virtual void Update() override;

    virtual void UpdateFromReference();

    void AddLabel(const String &content, int height = -1, int width = -1);
    void AddWidget(GameObject *widget,
                   int height = DefaultWidgetHeight);
    void AddWidget(const String &labelContent,
                   GameObject *widget,
                   int height = DefaultWidgetHeight);
    void RemoveWidget(GameObject *widget);

    void SetWidgetEnabled(GameObject *widget, bool enabled);

    int GetLabelsWidth() const;
    GameObject *GetWidgetsContainer() const;

protected:
    virtual InspectorWidgetTitle* CreateTitleGameObject();
    InspectorWidgetTitle *GetInspectorWidgetTitle() const;

private:
    int m_labelsWidth = -1;
    List<GameObject*> p_widgets;
    UMap<GameObject*, UILabel*> m_widgetToLabel;
    UMap<UILabel*, UILayoutElement*> m_labelToLabelLE;

    InspectorWidgetTitle *p_inspectorWidgetTitleGo = nullptr;
    UIImageRenderer *p_bgRenderer = nullptr;
    GameObject *p_widgetsContainer = nullptr;

    UILabel *CreateWidgetLabel(const String &content, int height, int width);

    void AddWidgetInternal(const String &labelContent,
                           GameObject *widget,
                           int height,
                           bool addLabel);
};

// Title GameObject for an InspectorWidget
class InspectorWidgetTitle : public GameObject
{
    GAMEOBJECT_EDITOR(InspectorWidgetTitle);

public:
    InspectorWidgetTitle();
    virtual ~InspectorWidgetTitle();

    UITextRenderer *GetText() const;
    UIImageRenderer *GetIcon() const;
    UITextRenderer *GetEnabledText() const;
    UICheckBox *GetEnabledCheckBox() const;

private:
    UITextRenderer *p_text = nullptr;
    UIImageRenderer *p_icon = nullptr;
    UITextRenderer *p_enabledText = nullptr;
    UICheckBox *p_enabledCheckBox = nullptr;

};

NAMESPACE_BANG_EDITOR_END

#endif // INSPECTORWIDGET_H

